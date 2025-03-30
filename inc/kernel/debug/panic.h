#ifndef _INC_KERNEL_DEBUG_PANIC_H_
#define _INC_KERNEL_DEBUG_PANIC_H_

#include <kernel/irq/exception_handler.h>

/**
 * @file kernel/debug/panic.h
 * @brief Kernel Panic
 */
namespace debug::panic {
	/**
	 * @fn void generate(const char msg[], int error = 0)
	 * @brief Generate panic
	 * @warning This will stop the whole system
	 */
	[[noreturn]] void generate(const char msg[], int error = 0);

	/**
	 * @fn void generateFromIRQ(const char msg[], irq::ExceptionContext* exceptionContext, int error = 0)
	 * @brief Generate panic (as viewed in exceptionContext)
	 * @warning This will stop the whole system
	 */
	[[noreturn]] void generateFromIRQ(const char msg[], irq::ExceptionContext* exceptionContext, int error = 0);
}

#endif /* ifndef _INC_KERNEL_DEBUG_PANIC_H_ */
