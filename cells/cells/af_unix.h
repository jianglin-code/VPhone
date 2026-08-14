/* SPDX-License-Identifier: LGPL-2.1+ */

#ifndef __LXC_AF_UNIX_H
#define __LXC_AF_UNIX_H

#include <stdio.h>
#include <sys/socket.h>
#include <stddef.h>
#include <sys/un.h>

#define KERNEL_SCM_MAX_FD 253

/* Allow the caller to set expectations. */

/*
 * UNIX_FDS_ACCEPT_EXACT will only succeed if the exact amount of fds has been
 * received  (unless combined with UNIX_FDS_ACCEPT_NONE).
 */
#define UNIX_FDS_ACCEPT_EXACT ((__u32)(1 << 0)) /* default */

/*
 * UNIX_FDS_ACCEPT_LESS will also succeed if less than the requested number of
 * fd has been received. If the UNIX_FDS_ACCEPT_NONE flag is not raised than at
 * least one fd must be received.
 * */
#define UNIX_FDS_ACCEPT_LESS ((__u32)(1 << 1))

/*
 * UNIX_FDS_ACCEPT_MORE will also succeed if more than the requested number of
 * fds have been received. Any additional fds will be silently closed.  If the
 * UNIX_FDS_ACCEPT_NONE flag is not raised than at least one fd must be
 * received.
 */
#define UNIX_FDS_ACCEPT_MORE ((__u32)(1 << 2)) /* wipe any extra fds */

/*
 * UNIX_FDS_ACCEPT_NONE can be specified with any of the above flags and
 * indicates that the caller will accept no file descriptors to be received.
 */
#define UNIX_FDS_ACCEPT_NONE ((__u32)(1 << 3))

/* UNIX_FDS_ACCEPT_MASK is the value of all the above flags or-ed together. */
#define UNIX_FDS_ACCEPT_MASK (UNIX_FDS_ACCEPT_EXACT | UNIX_FDS_ACCEPT_LESS | UNIX_FDS_ACCEPT_MORE | UNIX_FDS_ACCEPT_NONE)

/* Allow the callee to communicate reality. */

/* UNIX_FDS_RECEIVED_EXACT indicates that the exact number of fds was received. */
#define UNIX_FDS_RECEIVED_EXACT ((__u32)(1 << 16))

/*
 * UNIX_FDS_RECEIVED_LESS indicates that less than the requested number of fd
 * has been received.
 */
#define UNIX_FDS_RECEIVED_LESS ((__u32)(1 << 17))

/*
 * UNIX_FDS_RECEIVED_MORE indicates that more than the requested number of fd
 * has been received.
 */
#define UNIX_FDS_RECEIVED_MORE ((__u32)(1 << 18))

/* UNIX_FDS_RECEIVED_NONE indicates that no fds have been received. */
#define UNIX_FDS_RECEIVED_NONE ((__u32)(1 << 19))

/**
 * Defines a generic struct to receive file descriptors from unix sockets.
 * @fd_count_max : Either the exact or maximum number of file descriptors the
 *                 caller is willing to accept. Must be smaller than
 *                 KERNEL_SCM_MAX_FDs; larger values will be rejected.
 *                 Filled in by the caller.
 * @fd_count_ret : The actually received number of file descriptors.
 *                 Filled in by the callee.
 * @flags        : Flags to negotiate expectations about the number of file
 *                 descriptors to receive.
 *                 Filled in by the caller and callee. The caller's flag space
 *                 is UNIX_FDS_ACCEPT_* other values will be rejected. The
 *                 caller may only set one of {EXACT, LESS, MORE}. In addition
 *                 they can raise the NONE flag. Any combination of {EXACT,
 *                 LESS, MORE} will be rejected.
 *                 The callee's flag space is UNIX_FDS_RECEIVED_*. Only ever
 *                 one of those values will be set.
 * @fd           : Array to store received file descriptors into. Filled by the
 *                 callee on success. If less file descriptors are received
 *                 than requested in @fd_count_max the callee will ensure that
 *                 all additional slots will be set to -EBADF. Nonetheless, the
 *                 caller should only ever use @fd_count_ret to iterate through
 *                 @fd after a successful receive.
 */
struct unix_fds {
	__u32 fd_count_max;
	__u32 fd_count_ret;
	__u32 flags;
	__s32 fd[KERNEL_SCM_MAX_FD];
} __attribute__((aligned(8)));

/* does not enforce \0-termination */
extern int lxc_abstract_unix_open(const char *path, int type, int flags);
extern void lxc_abstract_unix_close(int fd);
/* does not enforce \0-termination */
extern int lxc_abstract_unix_connect(const char *path);

extern int lxc_abstract_unix_send_fds(int fd, const int *sendfds,
					       int num_sendfds, void *data,
					       size_t size);

extern int lxc_abstract_unix_send_fds_iov(int fd, const int *sendfds,
						   int num_sendfds,
						   struct iovec *iov,
						   size_t iovlen);

extern ssize_t lxc_abstract_unix_recv_fds(int fd,
						   struct unix_fds *ret_fds,
						   void *ret_data,
						   size_t size_ret_data);

extern ssize_t lxc_abstract_unix_recv_one_fd(int fd, int *ret_fd,
						      void *ret_data,
						      size_t size_ret_data);

extern int __lxc_abstract_unix_send_two_fds(int fd, int fd_first,
						     int fd_second, void *data,
						     size_t size);

static inline int lxc_abstract_unix_send_two_fds(int fd, int fd_first,
						 int fd_second)
{
	return __lxc_abstract_unix_send_two_fds(fd, fd_first, fd_second, NULL, 0);
}

extern ssize_t __lxc_abstract_unix_recv_two_fds(int fd, int *fd_first,
							 int *fd_second,
							 void *data, size_t size);

static inline ssize_t lxc_abstract_unix_recv_two_fds(int fd, int *fd_first, int *fd_second)
{
	return __lxc_abstract_unix_recv_two_fds(fd, fd_first, fd_second, NULL, 0);
}

extern int lxc_unix_send_fds(int fd, int *sendfds, int num_sendfds, void *data, size_t size);

extern int lxc_abstract_unix_send_credential(int fd, void *data, size_t size);

extern int lxc_abstract_unix_rcv_credential(int fd, void *data, size_t size);

extern int lxc_unix_sockaddr(struct sockaddr_un *ret, const char *path);
extern int lxc_unix_connect(struct sockaddr_un *addr);
extern int lxc_unix_connect_type(struct sockaddr_un *addr, int type);
extern int lxc_socket_set_timeout(int fd, int rcv_timeout, int snd_timeout);

/*
 * Compile time versions of __arch_hweightN()
 */
#define __const_hweight8(w)		\
	((unsigned int)			\
	 ((!!((w) & (1ULL << 0))) +	\
	  (!!((w) & (1ULL << 1))) +	\
	  (!!((w) & (1ULL << 2))) +	\
	  (!!((w) & (1ULL << 3))) +	\
	  (!!((w) & (1ULL << 4))) +	\
	  (!!((w) & (1ULL << 5))) +	\
	  (!!((w) & (1ULL << 6))) +	\
	  (!!((w) & (1ULL << 7)))))

#define __const_hweight16(w) (__const_hweight8(w)  + __const_hweight8((w)  >> 8 ))
#define __const_hweight32(w) (__const_hweight16(w) + __const_hweight16((w) >> 16))
#define __const_hweight64(w) (__const_hweight32(w) + __const_hweight32((w) >> 32))

#define hweight8(w) __const_hweight8(w)
#define hweight16(w) __const_hweight16(w)
#define hweight32(w) __const_hweight32(w)
#define hweight64(w) __const_hweight64(w)

#define close_prot_errno_disarm(fd) \
	if (fd >= 0) {              \
		int _e_ = errno;    \
		close(fd);          \
		errno = _e_;        \
		fd = -EBADF;        \
	}

#define MAX_ERRNO 4095

#define IS_ERR_VALUE(x) unlikely((x) >= (unsigned long)-MAX_ERRNO)

static inline void *ERR_PTR(long error)
{
	return (void *)error;
}

static inline long PTR_ERR(const void *ptr)
{
	return (long)ptr;
}

static inline long IS_ERR(const void *ptr)
{
	return IS_ERR_VALUE((unsigned long)ptr);
}

static inline long IS_ERR_OR_NULL(const void *ptr)
{
	return !ptr || IS_ERR_VALUE((unsigned long)ptr);
}

static inline void put_unix_fds(struct unix_fds *fds)
{
	if (!IS_ERR_OR_NULL(fds)) {
		for (size_t idx = 0; idx < fds->fd_count_ret; idx++)
			close_prot_errno_disarm(fds->fd[idx]);
	}
}
//define_cleanup_function(struct unix_fds *, put_unix_fds);

#endif /* __LXC_AF_UNIX_H */
