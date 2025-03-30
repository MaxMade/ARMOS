#ifndef _INC_KERNEL_ERRNO_H_
#define _INC_KERNEL_ERRNO_H_

#include <kernel/config.h>

/**
 * @file kernel/errno.h
 * @brief Global errno
 */

extern int __errnos[MAX_TID] __attribute__((section(".app.data")));

#endif /* ifndef _INC_KERNEL_ERRNO_H_ */
