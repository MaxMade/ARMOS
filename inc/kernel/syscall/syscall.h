#ifndef _INC_KERNEL_SYSCALL_SYSCALL_H_
#define _INC_KERNEL_SYSCALL_SYSCALL_H_

/**
 * @file kernel/syscall/syscall.h
 * @brief System Call Helper
 */

#include <cstddef.h>
#include <kernel/irq/exception_handler.h>

namespace syscall {

	/**
	 * @fn T getSyscallArg(irq::ExceptionContext* context)
	 * @brief Get N-th syscall argument
	 * @warning N must be in range [0, 5]
	 * @brief context Saved syscall context
	 */
	template<size_t N, typename T>
	T getSyscallArg(irq::ExceptionContext* context) {
		static_assert(N < 6, "Invalid argument number");

		if constexpr(N == 0)
			return (T) (uintptr_t) context->x0;

		if constexpr(N == 1)
			return (T) (uintptr_t) context->x1;

		if constexpr(N == 2)
			return (T) (uintptr_t) context->x2;

		if constexpr(N == 3)
			return (T) (uintptr_t) context->x3;

		if constexpr(N == 4)
			return (T) (uintptr_t) context->x4;

		if constexpr(N == 5)
			return (T) (uintptr_t) context->x5;
	}

	/**
	 * @fn void setSyscallRetValue(irq::ExceptionContext* context, T value)
	 * @brief Set syscall return value
	 */
	template<typename T>
	void setSyscallRetValue(irq::ExceptionContext* context, T value) {
		context->x0 = (uintptr_t) value;
	}

	/**
	 * @fn bool isReadable(const void* buf, size_t size)
	 * @brief Check if buffer is readable by user thread
	 */
	bool isReadable(const void* buf, size_t size);

} /* namespace syscall */

#endif /* ifndef _INC_KERNEL_SYSCALL_SYSCALL_H_ */
