/*
 * cell_console.c
 *
 * Routines supporting consoles on pseudo terminals within a cell
 *
 * Copyright (C) 2010-2013 Columbia University
 * Authors: Christoffer Dall <cdall@cs.columbia.edu>
 *		  Jeremy C. Andrus <jeremya@cs.columbia.edu>
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
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>

#define LOG_TAG "Cells/console"
#include <cutils/log.h>

#include "cell_console.h"

#define PRINT_BUF_LEN 256
static void cell_console_rw_loop(int ptm, const char * cmd, const char * args)
{
	fd_set rfds;
	fd_set efds;
	char buf[PRINT_BUF_LEN];
	int ret;
	//char cons[32];

	//snprintf(cons, sizeof(cons), "PS1=%s$PS1", args);

	FD_ZERO(&rfds);
	FD_ZERO(&efds);

	while (1) {
		FD_SET(0, &rfds);
		FD_SET(ptm, &rfds);
		FD_SET(ptm, &efds);

		ret = select(ptm + 1, &rfds, NULL, &efds, NULL);
		if (ret < 0 && errno != EINTR) {
			ALOGE("select error:%d", errno);
			break;
		}

		if (FD_ISSET(0, &rfds)) {
			/* Read something from stdin and write to pty */
			if ((ret = read(0, buf, PRINT_BUF_LEN)) > 0) {
				if (ret == 1 && buf[0] == 0x4) {
					/* Catch ctrl+d control code */
					kill(getppid(), SIGINT);
					break;
				}
				write(ptm, buf, ret);
			}
		}
		if (FD_ISSET(ptm, &rfds)) {
			/* Read something from pty and write to stdout */
			ret = read(ptm, buf, PRINT_BUF_LEN);
			if (ret > 0)
				write(1, buf, ret);
			if (cmd && (ret <= 0 || buf[ret-1] == 0x4))
				break;
		}
		if (FD_ISSET(ptm, &efds)) {
			ALOGE("console ptm:%d error: exiting", ptm);
			break;
		}
	}

	if (ret < 0)
		ALOGE("read tty's ptm");
}

static void console_run_cmd(int ptm, const char *cmd)
{
	fd_set rfds;
	fd_set efds;
	char buf[PRINT_BUF_LEN];
	int ret;

	FD_ZERO(&rfds);
	FD_ZERO(&efds);
	while (1) {
		FD_SET(0, &rfds);
		FD_SET(ptm, &rfds);
		FD_SET(ptm, &efds);

		ret = select(ptm + 1, &rfds, NULL, &efds, NULL);
		if (ret < 0 && errno != EINTR) {
			ALOGE("select error:%d", errno);
			break;
		}

		if (FD_ISSET(0, &rfds)) {
			/* Read something from stdin and write to pty */
			if ((ret = read(0, buf, PRINT_BUF_LEN)) > 0) {
				if (ret == 1 && buf[0] == 0x4) {
					/* Catch ctrl+d control code */
					kill(getppid(), SIGINT);
					break;
				}
				write(ptm, buf, ret);
			}
		}
		if (FD_ISSET(ptm, &rfds)) {
			/* Read something from pty and write to stdout */
			if ((ret = read(ptm, buf, PRINT_BUF_LEN)) > 0)
				write(1, buf, ret);
		}
		if (FD_ISSET(ptm, &efds)) {
			ALOGE("console ptm:%d error: exiting", ptm);
			break;
		}
	}

	if (ret < 0)
		ALOGE("read tty's ptm");
}

/**
 * Opens a console using a pseudo-terminal master file descriptor.
 *
 * Catches ctrl+d to exit the container gracefully.
 *
 * @ptm: The ptm fd associated with the console
 * @cmd: an initial command to send to the console
 */
int open_cell_console(int ptm, const char *cmd, const char *args)
{
	struct termios oldtios, newtios;
	//char cons[32];

	if(ptm == -1)
		return -1;

	/* Save current terminal settings */
	if (tcgetattr(STDIN_FILENO, &oldtios) < 0) {
		ALOGE("cannot get stdin termios attr");
		return -1;
	}

	/* Copy the old terminal settings and change required bits */
	memcpy(&newtios, &oldtios, sizeof(struct termios));
	newtios.c_iflag &= ~IGNBRK;
	newtios.c_iflag &= BRKINT;
	newtios.c_lflag &= ~(ECHO|ICANON|ISIG);
	newtios.c_cc[VMIN] = 1;
	newtios.c_cc[VTIME] = 0;

	/* Set new terminal settings */
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &newtios) < 0) {
		ALOGE("set stdin termoios attr");
		return -1;
	}

	//snprintf(cons, sizeof(cons), "PS1=%s$PS1", args);
	//write(ptm, cons, strlen(cons));

	/* Do console i/o to the terminal until a signal or EOF */
	cell_console_rw_loop(ptm, cmd, args);

	/* Reset the terminal settings */
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldtios) < 0) {
		ALOGE("set stdin termoios attr");
		return -1;
	}
	return 0;
}

/**
 * Closes file descriptors and conatiner mounted slave pts.
 * Should be called when the container has exited.
 */
void cleanup_cell_console(struct pty_info *pi)
{
	close(pi->ptm);
	close(pi->pty);
	/*printf("Unmounting %s\n", pi->cont_path);*/
	umount2(pi->cont_path, MNT_FORCE); /* Ignore failure */
}

int create_vm_console(const char *ptss)
{
    //char name_slave[50] = {0};
    //int ptm;
    static pid_t pid = 0;

	if(pid > 0)
	{
        errno = 0;
        kill(pid, SIGKILL);
        ALOGD("kill vm console : %d %s", pid, strerror(errno));
		pid = 0;
	}

	ALOGD("create vm console");

    /*ptm = open("/pts/ptmx", O_RDWR|O_CLOEXEC); // | O_NOCTTY);
    if(ptm < 0){
        ALOGE("cannot open /dev/ptmx - %s", strerror(errno));
        return -1;
    }
    //fcntl(ptm, F_SETFD, FD_CLOEXEC);

    if(grantpt(ptm) || unlockpt(ptm) ||
       (ptsname_r(ptm, name_slave, sizeof(name_slave))!=0)){
        ALOGE("trouble with /dev/ptmx - %s", strerror(errno));
        return -1;
    }*/

	ALOGD("vm name_slave = %s", strstr(ptss,"/pts"));

    errno = 0;
    pid = fork();
    if(pid < 0) {
        ALOGE("- fork failed: %s -", strerror(errno));
        return -1;
    }

    if(pid == 0){
        setsid();

		errno = 0;
        int pts = open(strstr(ptss,"/pts"), O_RDWR|O_CLOEXEC);
        if(pts < 0){
			ALOGD("pts = %d %s",pts,strerror(errno));
			exit(-1);
		}

        //ioctl(pts, TIOCSCTTY, 0);

        dup2(pts, STDIN_FILENO);
        dup2(pts, STDOUT_FILENO);
        dup2(pts, STDERR_FILENO);

		close(pts);
		//close(ptm);

		ALOGD("execl /system/bin/sh");
        execl("/system/bin/sh","/system/bin/sh","-",NULL);
		ALOGE("/system/bin/sh failed: %s", strerror(errno));
        exit(-1);
    } else {
        return 1;
    }
}
