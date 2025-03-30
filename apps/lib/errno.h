#ifndef _INC_APPS_LIB_ERRNO_H_
#define _INC_APPS_LIB_ERRNO_H_

#include <pthread.h>

extern int __errnos[] __attribute__((section(".app.data")));

#define errno __errnos[(long) pthread_self()]

#endif /* ifndef _INC_APPS_LIB_ERRNO_H_ */
