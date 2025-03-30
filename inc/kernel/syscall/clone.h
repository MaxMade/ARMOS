#ifndef _INC_KERNEL_SYSCALL_CLONE_H_
#define _INC_KERNEL_SYSCALL_CLONE_H_

/**
 * @file kernel/syscall/clone.h
 * @brief Clone System Call
 */

#include <kernel/irq/exception_handler.h>

#include <cstddef.h>

namespace syscall {

	/**
	 * @fn int clone(int (*fn)(void *), void *stack, int flags, void* arg)
	 * @brief Clone system call
	 * @warning The implementation will only spawn threads in the same context
	 */
	int clone(int (*fn)(void *), void *stack, int flags, void* arg);

	/**
	 * @fn void __clone(irq::ExceptionContext* irq)
	 * @brief Clone system call wrapper
	 */
	void __clone(irq::ExceptionContext* irq);

} /* namespace syscall */

#endif /* ifndef _INC_KERNEL_SYSCALL_CLONE_H_ */
