#ifndef _APP_LIB_UNISTD_H_
#define _APP_LIB_UNISTD_H_

/**
 * @file apps/lib/unistd.h
 * @brief Standard symbolic constants and types
 */

/**
 * @def NULL
 * @brief Null pointer constant
 */
#ifndef NULL
	#define NULL ((void *) 0)
#endif

/**
 * @typedef size_t
 * @brief Unsigned integer type of the result of \c sizeof operator
 */
#ifndef size_t
	typedef unsigned long size_t;
#endif

/**
 * @typedef ssize_t
 * @brief Signed integer type
 */
#ifndef ssize_t
	typedef long ssize_t;
#endif

/**
 * @fn long syscall(long number, T0 t0 = 0, T1 t1 = 0, T2 t2 = 0, T3 t3 = 0, T4 t4 = 0, T5 t5 = 0)
 * @brief Indirect system cal
 */
template<typename T0 = void*, typename T1 = void*, typename T2 = void*, typename T3 = void*, typename T4 = void*, typename T5 = void*>
long syscall(long number, T0 t0 = 0, T1 t1 = 0, T2 t2 = 0, T3 t3 = 0, T4 t4 = 0, T5 t5 = 0) {
	int ret = 0;

	/* Use 64 bit register */
	auto _t0 = (long long) t0;
	auto _t1 = (long long) t1;
	auto _t2 = (long long) t2;
	auto _t3 = (long long) t3;
	auto _t4 = (long long) t4;
	auto _t5 = (long long) t5;

	/* Perform system call */
	asm(
			"ldr x8, %[num]\n\t"
			"ldr x0, %[arg0]\n\t"
			"ldr x1, %[arg1]\n\t"
			"ldr x2, %[arg2]\n\t"
			"ldr x3, %[arg3]\n\t"
			"ldr x4, %[arg4]\n\t"
			"ldr x5, %[arg5]\n\t"
			"svc #0\n\t"
			"str x0, %[ret]\n\t"
			: [ret] "=m"(ret)
			: [num] "m"(number), [arg0] "m"(_t0), [arg1] "m"(_t1), [arg2] "m"(_t2),
			  [arg3] "m"(_t3), [arg4] "m"(_t4), [arg5] "m"(_t5)
			: "x0", "x1", "x2", "x3", "x4", "x5", "x8"
	   );

	return ret;
}

#endif /* ifndef _APP_LIB_UNISTD_H_ */
