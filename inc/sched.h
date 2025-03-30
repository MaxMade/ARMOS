#ifndef _INC_SCHED_H_
#define _INC_SCHED_H_

/**
 * @file sched.h
 * @brief Execution scheduling
 */

namespace lib {

	/**
	 * @enum CLONE_FLAGS
	 * @brief Flags passed to the clone system call
	 */
	enum CLONE_FLAGS {
		CLONE_VM             = 0x00000100, /* Calling process and child process run in the same memory space */
		CLONE_FS             = 0x00000200, /* Calling process and child process share the same filesystem information */
		CLONE_FILES          = 0x00000400, /* Calling process and child process share the same file descriptor table */
		CLONE_SIGHAND        = 0x00000800, /* Calling process and child process share the same signal handler table */
		CLONE_PIDFD          = 0x00001000, /* PID file descriptor referring to child process is allocated in calling's memory */
		CLONE_PTRACE         = 0x00002000, /* Trace child process if calling process is traced */
		CLONE_VFORK          = 0x00004000, /* Execution of calling process is supsended till child releases its virtual memory */
		CLONE_PARENT         = 0x00008000, /* Calling process and child process share same PPID */
		CLONE_THREAD         = 0x00010000, /* New thread in thread group of calling process */
		CLONE_NEWNS          = 0x00020000, /* Child process is started in new mount namespace */
		CLONE_SYSVSEM        = 0x00040000, /* Calling process and child process share the same System V semaphore adjustment */
		CLONE_SETTLS         = 0x00080000, /* The TLS (thread local storage) descriptor is set to tls */
		CLONE_PARENT_SETTID  = 0x00100000, /* Set the TID in the calling process */
		CLONE_CHILD_CLEARTID = 0x00200000, /* Clear the TID in the child process */
		CLONE_CHILD_SETTID   = 0x01000000, /* Set the TID in the child process */
		CLONE_NEWCGROUP      = 0x02000000, /* Child process is started in a new cgroup namespace */
		CLONE_NEWUTS         = 0x04000000, /* Child process is started in a new utsname namespace */
		CLONE_NEWIPC         = 0x08000000, /* Child process is started in a new ipc namespace */
		CLONE_NEWUSER        = 0x10000000, /* Child process is started in a new user namespace */
		CLONE_NEWPID         = 0x20000000, /* Child process is started in a new pid namespace */
		CLONE_NEWNET         = 0x40000000, /* Child process is started in a new network namespace */
		CLONE_IO             = 0x80000000, /* Calling process and child process share the same io context */
	};

} /* namespace lib */

#endif /* ifndef _INC_SCHED_H_ */
