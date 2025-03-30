#ifndef _INC_KERNEL_SYSCALL_WRITE_H_
#define _INC_KERNEL_SYSCALL_WRITE_H_

/**
 * @file kernel/syscall/write.h
 * @brief Write System Call
 */

#include <kernel/irq/exception_handler.h>

#include <cstddef.h>

namespace syscall {

	/**
	 * @fn ssize_t write(int fd, const void* buf, size_t count)
	 * @brief Write system call
	 */
	ssize_t write(int fd, const void* buf, size_t count);

	/**
	 * @fn void __write(irq::ExceptionContext* irq)
	 * @brief Write system call wrapper
	 */
	void __write(irq::ExceptionContext* irq);

} /* namespace syscall */

#endif /* ifndef _INC_KERNEL_SYSCALL_WRITE_H_ */
