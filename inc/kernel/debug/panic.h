#ifndef _INC_KERNEL_DEBUG_PANIC_H_
#define _INC_KERNEL_DEBUG_PANIC_H_

/**
 * @file kernel/debug/panic.h
 * @brief Kernel Panic
 */
namespace debug::panic {

	/**
	 * @fn int init()
	 * @brief Initialize Panic Handler
	 */
	int init();

	/**
	 * @fn void generate(const char msg[])
	 * @brief Generate panic
	 * @warning This will stop the whole system
	 */
	void generate(const char msg[]);
}

#endif /* ifndef _INC_KERNEL_DEBUG_PANIC_H_ */
