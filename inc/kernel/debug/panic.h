#ifndef _INC_KERNEL_DEBUG_PANIC_H_
#define _INC_KERNEL_DEBUG_PANIC_H_

#include <kernel/irq/exception_handler.h>

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

	/**
	 * @fn void generateFromIRQ(const char msg[], irq::ExceptionContext* exceptionContext)
	 * @brief Generate panic (as viewed in exceptionContext)
	 * @warning This will stop the whole system
	 */
	void generateFromIRQ(const char msg[], irq::ExceptionContext* exceptionContext);
}

#endif /* ifndef _INC_KERNEL_DEBUG_PANIC_H_ */
