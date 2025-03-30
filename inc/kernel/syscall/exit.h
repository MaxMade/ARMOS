#ifndef _INC_KERNEL_SYSCALL_EXIT_H_
#define _INC_KERNEL_SYSCALL_EXIT_H_

/**
 * @file kernel/syscall/exit.h
 * @brief Exit System Call
 * @warning The exit system call halt the system and print the status to cout.
 */

#include <kernel/irq/exception_handler.h>

#include <cstddef.h>

namespace syscall {

	/**
	 * @fn [[noreturn]] void exit(int status)
	 * @brief Exit system call
	 */
	[[noreturn]] void exit(int status);

	/**
	 * @fn [[noreturn]] void exit(int status)
	 * @brief Exit system call wrapper
	 */
	[[noreturn]] void __exit(irq::ExceptionContext* irq);

} /* namespace syscall */

#endif /* ifndef _INC_KERNEL_SYSCALL_EXIT_H_ */
