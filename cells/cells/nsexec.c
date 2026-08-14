/*
 * nsexec.c
 * Based on nsexec.c which is Copyright (C) 2008,2009 IBM Corp.
 *
 * routines for cloning a cell in a new set of namespaces
 *
 * Copyright (C) 2010-2013 Columbia University
 * Authors: Christoffer Dall <cdall@cs.columbia.edu>
 *		  Jeremy C. Andrus <jeremya@cs.columbia.edu>
 *		  Alexander Van't Hof <alexvh@cs.columbia.edu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */
#define _GNU_SOURCE
#include <sched.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/sched.h>
#include <sys/capability.h>
#include <sys/prctl.h>

#define LOG_TAG "Cells/nsexec"
#include <cutils/log.h>
#include <cutils/memory.h>
#include <cutils/misc.h>
#include <selinux/selinux.h>
#include "cutils/properties.h"
#include <cutils/android_filesystem_config.h>

#include "celld.h"
#include "cell_console.h"
#include "util.h"
#include "cellnetwork.h"

#ifndef MNT_DETACH
#define MNT_DETACH 2
#endif

/* Linux clone flags not available in bionic's kernel headers */
#ifndef CLONE_NEWUTS
#define CLONE_NEWUTS			0x04000000	  /* New utsname group? */
#endif
#ifndef CLONE_NEWIPC
#define CLONE_NEWIPC			0x08000000	  /* New ipcs */
#endif
#ifndef CLONE_NEWUSER
#define CLONE_NEWUSER		   0x10000000	  /* New user namespace */
#endif
#ifndef CLONE_NEWPID
#define CLONE_NEWPID			0x20000000	  /* New pid namespace */
#endif
#ifndef CLONE_NEWNET
#define CLONE_NEWNET			0x40000000	  /* New network namespace */
#endif
#ifndef CLONE_IO
#define CLONE_IO				0x80000000	  /* Clone io context */
#endif
#ifndef CLONE_NEWCGROUP
#define CLONE_NEWCGROUP		0x02000000
#endif

/*
 * cloning flags intersect with CSIGNAL so can be used with unshare and clone3
 * syscalls only:
 */
#ifndef CLONE_NEWTIME
#define CLONE_NEWTIME	0x00000080	/* New time namespace */
#endif

/*
#ifndef __NR_clone3
#define __NR_clone3 435
#endif
*/

struct __clone_args {
	__aligned_u64 flags;
	__aligned_u64 pidfd;
	__aligned_u64 child_tid;
	__aligned_u64 parent_tid;
	__aligned_u64 exit_signal;
	__aligned_u64 stack;
	__aligned_u64 stack_size;
	__aligned_u64 tls;
#ifndef CLONE_ARGS_SIZE_VER0
#define CLONE_ARGS_SIZE_VER0 64	/* sizeof first published struct */
#endif
	__aligned_u64 set_tid;
	__aligned_u64 set_tid_size;
#ifndef CLONE_ARGS_SIZE_VER1
#define CLONE_ARGS_SIZE_VER1 80	/* sizeof second published struct */
#endif
	__aligned_u64 cgroup;
#ifndef CLONE_ARGS_SIZE_VER2
#define CLONE_ARGS_SIZE_VER2 88	/* sizeof third published struct */
#endif
};

static void drop_cap_sys(struct cell_args *args) {
	if(!args->start_args.set_time) {
		if (prctl(PR_CAPBSET_DROP, CAP_SYS_TIME) == -1) {
			ALOGE("prctl PR_CAPBSET_DROP CAP_SYS_TIME");
		}
	}

	if (prctl(PR_CAPBSET_DROP, CAP_SYS_BOOT) == -1) {
        ALOGE("prctl PR_CAPBSET_DROP CAP_SYS_BOOT");
    }
}

static bool switch_timens(void)
{
	int fd, ret;

	ALOGD("switch time");

	if (unshare(CLONE_NEWTIME) == -1)
		return false;

	fd = open("/proc/self/ns/time_for_children", O_RDONLY | O_CLOEXEC);
	if (fd < 0)
		return false;

	ret = setns(fd, CLONE_NEWTIME);
	close(fd);

	if (ret != 0)
		ALOGE("switch time failed!!!");

	return ret == 0;
}

static long clone3(struct __clone_args *cl_args)
{
    return syscall(__NR_clone3, cl_args, sizeof(struct __clone_args));
}

extern int clone(int (*fn)(void *), void *child_stack, int flags, void *arg, ...);

static int rootfs_chroot_root(const char *rootfs)
{
	int i, ret;

	ret = chdir("/");
	if (ret < 0){
		ALOGD("Failed chdir /.");
		return -1;
	}

	/* We could use here MS_MOVE, but in userns this mount is locked and
	 * can't be moved.
	 */
	ret = mount(rootfs, "/", NULL, MS_REC | MS_BIND, NULL);
	if (ret < 0){
		ALOGD("Failed to mount %s onto / as MS_REC | MS_BIND.", rootfs);
		return -1;
	}

	ret = mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL);
	if (ret < 0){
		ALOGD("Failed to remount / .");
		return -1;
	}

	/* The following code cleans up inherited mounts which are not required
	 * for CT.
	 *
	 * The mountinfo file shows not all mounts, if a few points have been
	 * unmounted between read operations from the mountinfo. So we need to
	 * read mountinfo a few times.
	 *
	 * This loop can be skipped if a container uses userns, because all
	 * inherited mounts are locked and we should live with all this trash.
	 */
	for (;;) {
		FILE *f = NULL;
		char *line = NULL;
		char *slider1, *slider2;
		int progress = 0;
		size_t len = 0;

		f = fopen("./proc/self/mountinfo", "re");
		if (!f){
			ALOGD("Failed to open /proc/self/mountinfo .");
			return -1;
		}

		while (getline(&line, &len, f) > 0) {
			for (slider1 = line, i = 0; slider1 && i < 4; i++)
				slider1 = strchr(slider1 + 1, ' ');

			if (!slider1)
				continue;

			slider2 = strchr(slider1 + 1, ' ');
			if (!slider2)
				continue;

			*slider2 = '\0';
			*slider1 = '.';

			if (strcmp(slider1 + 1, "/") == 0)
				continue;

			if (strcmp(slider1 + 1, "/proc") == 0)
				continue;

			ret = umount2(slider1, MNT_DETACH);
			if (ret == 0)
				progress++;
		}

		if (!progress){
			fclose(f);
			break;
		}
	}

	/* This also can be skipped if a container uses userns. */
	(void)umount2("./proc", MNT_DETACH);

	/* It is weird, but chdir("..") moves us in a new root */
	ret = chdir("..");
	if (ret < 0){
		ALOGD("Failed to chdir(..).");
		return -1;
	}

	ret = chroot(".");
	if (ret < 0){
		ALOGD("Failed to chroot(.).");
		return -1;
	}

	return 0;
}

static int do_child(void *vargv)
{
	struct cell_args *cell_args = (struct cell_args *)vargv;
	struct cell_start_args *start_args = &cell_args->start_args;
	char **argv;
	char *rootdir;
	char *cellname;
	char *syserr;
	int ret;
	char buf[20];
	sigset_t sigset;

	argv = cell_args->argv;
	cellname = cell_args->cellname;
	rootdir = cell_args->rootdir;

	errno = 0;
	int nsfd = open("/proc/drv_ns/ns_tag", O_WRONLY);
	if (nsfd >= 0) {
		ret = write(nsfd, cellname, strlen(cellname));
		close(nsfd);
		ALOGD("/proc/drv_ns/ns_tag ret = %d errno = %s",ret,strerror(errno));
	}else{
		ALOGE("/proc/drv_ns/ns_tag nsfd = %d errno = %s",nsfd,strerror(errno));
	}

	ALOGD("Starting cell:");
	ALOGD("==============");
	ALOGD("start_args");
	ALOGD("----------");
	ALOGD("noopt: %d", start_args->noopt);
	ALOGD("uts_ns: %d", start_args->uts_ns);
	ALOGD("ipc_ns: %d", start_args->ipc_ns);
	ALOGD("user_ns: %d", start_args->user_ns);
	ALOGD("net_ns: %d", start_args->net_ns);
	ALOGD("pid_ns: %d", start_args->pid_ns);
	ALOGD("mount_ns: %d", start_args->mount_ns);
	ALOGD("mnt_rootfs: %d", start_args->mnt_rootfs);
	ALOGD("newcgrp: %d", start_args->newcgrp);
	ALOGD("open_console: %d", start_args->open_console);
	ALOGD("autoswitch: %d", start_args->autoswitch);
	ALOGD("pid_file: %s", start_args->pid_file);
	ALOGD("wait: %d", start_args->wait);
	ALOGD("\ncell_args");
	ALOGD("---------");
	ALOGD("cellname: %s", cell_args->cellname);
	ALOGD("cellindex: %d", cell_args->cellindex);
	ALOGD("rootdir: %s", cell_args->rootdir);
	ALOGD("argc: %d  argv: %s", cell_args->argc, cell_args->argv[0]);

	/* reset out umask and sigmask for init */
	umask(0000);
	sigemptyset(&sigset);
	sigprocmask(SIG_SETMASK, &sigset, NULL);

	/* Make sure init doesn't kill CellD on bad cell errors */
	ret = setpgid(0, 0);
	if (ret < 0)
		ALOGE("error setting pgid: %s", strerror(errno));

	ALOGI("%s: do_child, mnt_rootfs:%d, rootdir=%s",
		  cellname, start_args->mnt_rootfs, rootdir);

	ALOGD("%s: waiting for CellD...", cellname);
	close(cell_args->init_pipe[1]);
	ret = read(cell_args->init_pipe[0], buf, sizeof(buf));
	close(cell_args->init_pipe[0]);
	if (ret == -1 || buf[0] != 49) {
		syserr = "CellD communication";
		goto out_err;
	}

	if (start_args->net_ns){
		nssetvirt(cell_args->scellscmds);
	}

	if(rootfs_chroot_root(rootdir) != 0){
		syserr = "chroot";
		goto out_err;
	}

	ALOGD("%s: Starting init!", cellname);

	int fdcell = open("/.cell",O_WRONLY|O_CREAT,0660);
	if(fdcell >= 0){
		char value[PROPERTY_VALUE_MAX] = {0};
		property_get("persist.sys.exit", value, "1");
		if (strcmp(value, "0") == 0) {
			write(fdcell, "1", 1);
		}else{
			write(fdcell, "0", 1);
		}
		close(fdcell);
		fdcell = 0;
	}

	int fdname = open("/.name",O_WRONLY|O_CREAT,0660);
	if(fdname >= 0){
		write(fdname, cellname, strlen(cellname));
		close(fdname);
		fdname = 0;
	}

	int fdwifi = open("/.wifi",O_WRONLY|O_CREAT,0660);
	if(fdwifi >= 0){
		const char* value = getwifiproxy(cell_args->scellscmds);
		if(value){
			ALOGD("wifiproxy: %s", value);
			write(fdwifi, value, strlen(value));
		}
		close(fdwifi);
		fdwifi = 0;
	}

	int fdrmnet = open("/.rmnet",O_WRONLY|O_CREAT,0660);
	if(fdrmnet >= 0){
		const char* value = getrmnetproxy(cell_args->scellscmds);
		if(value){
			ALOGD("rmnetproxy: %s", value);
			write(fdrmnet, value, strlen(value));
		}
		close(fdrmnet);
		fdrmnet = 0;
	}

	int fdenforce = open("/.enforce",O_WRONLY|O_CREAT,0660);
	if(fdenforce >= 0){
		char value[PROPERTY_VALUE_MAX] = {0};
		property_get("persist.sys.host.selinux.enforce", value, "0");
		ALOGD("persist.sys.host.selinux.enforce: %s", value);
		write(fdenforce, value, 1);
		close(fdenforce);
		fdenforce = 0;
	}

	for (int fd = 3; fd < sysconf(_SC_OPEN_MAX); fd++) {
		fcntl(fd, F_SETFD, FD_CLOEXEC);
	}

	close(cell_args->child_pipe[0]);
	write(cell_args->child_pipe[1], "1", 1);
	close(cell_args->child_pipe[1]);

	execve(cell_args->argv[0], cell_args->argv, NULL);
	syserr = "execve";

out_err:
	{
		int e = errno;
		ALOGE("ERROR{%s: errno=%d (%s)}", syserr, e, strerror(e));
	}
	return -1;
}

static int write_pid(char *pid_file, int pid)
{
	FILE *fp;

	if (!pid_file)
		return 0;

	fp = fopen(pid_file, "w");
	if (!fp)
		return -1;
	fprintf(fp, "%d", pid);
	fflush(fp);
	fclose(fp);
	return 0;
}

static int do_clone(struct cell_args *cell_args)
{
	struct cell_start_args *args = &cell_args->start_args;
	struct __clone_args cl_args = {0};
	int pid;
	unsigned long flags = 0;

	char value[PROPERTY_VALUE_MAX] = {0};
	property_get("persist.sys.host.selinux.enforce", value, "0");
	ALOGD("persist.sys.host.selinux.enforce: %s", value);

	//flags = SIGCHLD;
	if (args->uts_ns)
		flags |= CLONE_NEWUTS;
	if (args->ipc_ns)
		flags |= CLONE_NEWIPC;
	if (args->user_ns)
		flags |= CLONE_NEWUSER;
	if (args->net_ns)
		flags |= CLONE_NEWNET;
	if (args->pid_ns)
		flags |= CLONE_NEWPID;
	if (args->mount_ns)
		flags |= CLONE_NEWNS;
	if (args->newcgrp)
		flags |= CLONE_NEWCGROUP;

	//flags |= CLONE_NEWTIME;

	//pid = clone(do_child, childstack, flags, cell_args);

	cl_args.flags = flags;
	cl_args.stack = 0;//(__u64)stack;
	cl_args.stack_size = 0;//stacksize;
	cl_args.exit_signal = SIGCHLD;

	pid = clone3(&cl_args);

	if (pid < 0) {
		ALOGE("clone: %s", strerror(errno));
		return -1;
	} else if (pid == 0){
		switch_timens();
		drop_cap_sys(cell_args);
		do_child(cell_args);
		exit(0);//child process exit 
	}

	if (gettimeofday(&cell_args->start_time, NULL) == -1)
		ALOGE("%s: gettimeofday failed: %s", __func__, strerror(errno));

	/*
	 * Put the new process in a cgroup if requested.
	 * Note that the child will block until we release it with a write
	 * into the global pipe. This ensures that all children of the cell's
	 * init process will inherit the cgroup (i.e. a child will _not_ be
	 * forked before we can put init into a cgroup)
	 */
	cell_args->init_pid = pid;
	return pid;
}

/* Careful: When this is called, it's called from a different process.
 * That means, no signaling to celld's threads (ex: destroy_wifi) */
void tear_down_cell(struct cell_args *cell_args)
{
	struct cell_start_args *args = &cell_args->start_args;

	if (unmount_all(cell_args->rootdir, args->mnt_rootfs) == -1)
		ALOGW("Couldn't unmount_all() on %s", cell_args->rootdir);
}

static void thread_exit_handler(int sig)
{
	pthread_exit(0);
}

/* sd is used for sending more detailed error messages to client. */
int cell_nsexec(int sd, struct cell_args *cell_args, char *name)
{
	struct cell_start_args *args = &cell_args->start_args;
	int pid = -1;
	char *rootdir = cell_args->rootdir;
	struct sigaction actions;

	/* Setup signal handler for SIGUSR2 (fake pthread_cancel) */
	memset(&actions, 0, sizeof(actions));
	sigemptyset(&actions.sa_mask);
	actions.sa_flags = 0;
	actions.sa_handler = thread_exit_handler;
	if (sigaction(SIGUSR2, &actions, NULL) < 0)
		ALOGE("sigaction(%s): %s", name, strerror(errno));

	/* pipes to synchronize child start and CellD monitoring */
	if (pipe(cell_args->child_pipe) || pipe(cell_args->init_pipe)) {
		ALOGE("Can't create child/init pipes for '%s': %s",
			  name, strerror(errno));
		send_msg(sd, "0 nsexec failed: child/init pipe creating failed");
		goto err_cleanup;
	}

	if (args->mnt_rootfs) {
		if (mount_cell(name)) {
			ALOGE("couldn't mount '%s' rootfs: %d", name, errno);
			send_msg(sd, "0 nsexec failed: mount() rootfs failed");
			goto err_cleanup;
		}
	}

	if(args->net_ns){
		cell_args->scellscmds = createvirtnetwork(name);
		createvirt(cell_args->scellscmds);
	}

	ALOGI("Cloning '%s'", name);
	pid = do_clone(cell_args);

	if (pid == -1) {
		ALOGE("clone(%s) failed: tearing down cell", name);
		goto err_cleanup;
	}

	write_pid(args->pid_file, pid);

	if(args->net_ns){
		mvvirttons(cell_args->scellscmds, pid);
		starttethervirt(cell_args->scellscmds);
	} // mast be before

	close(cell_args->init_pipe[0]);
	write(cell_args->init_pipe[1], "1", 1);
	close(cell_args->init_pipe[1]);

	close(cell_args->child_pipe[1]);

	ALOGI("Successfully initialized '%s' with init PID %d", name, pid);
	return pid;

err_cleanup:
	tear_down_cell(cell_args);
	return -1;
}
