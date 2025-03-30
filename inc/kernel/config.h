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

#endif /* ifndef _INC_KENREL_CONFIG_H_ */
