#ifndef _INC_KENREL_CONFIG_H_
#define _INC_KENREL_CONFIG_H_ value

/**
 * @file kernel/config.h
 * @brief Kernel configuration options
 */

/**
 * @def MAX_NUM_CPUS
 * @brief Max. number of supported CPUs
 */
#if defined(CONFIG_MAX_CPU_4)
	#define MAX_NUM_CPUS 4

#elif defined(CONFIG_MAX_CPU_8)
	#define MAX_NUM_CPUS 8

#elif defined(CONFIG_MAX_CPU_16)
	#define MAX_NUM_CPUS 16

#elif defined(CONFIG_MAX_CPU_32)
	#define MAX_NUM_CPUS 32

#elif defined(CONFIG_MAX_CPU_64)
	#define MAX_NUM_CPUS 64

#else
	#define MAX_NUM_CPUS 128
#endif

/**
 * @def STACK_SIZE
 * @brief Stack size in bytes
 */
#if defined(CONFIGSTACK_SIZE_16)
	#define STACK_SIZE (16 * 1024)

#elif defined(CONFIGSTACK_SIZE_32)
	#define STACK_SIZE (32 * 1024)

#elif defined(CONFIGSTACK_SIZE_64)
	#define STACK_SIZE (64 * 1024)

#elif defined(CONFIGSTACK_SIZE_128)
	#define STACK_SIZE (128 * 1024)

#elif defined(CONFIGSTACK_SIZE_256)
	#define STACK_SIZE (256 * 1024)

#elif defined(CONFIGSTACK_SIZE_512)
	#define STACK_SIZE (512 * 1024)

#else
	#define STACK_SIZE (1024 * 1024)
#endif

#endif /* ifndef _INC_KENREL_CONFIG_H_ */
