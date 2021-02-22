#ifndef _INC_KERNEL_IRQ_EXCEPTION_VECTOR_H_
#define _INC_KERNEL_IRQ_EXCEPTION_VECTOR_H_ value

/**
 * @file kernel/irq/exception_handler.h
 * @brief Operations concerning the exception handlers
 */

#include <cstdint.h>

namespace irq {

	/**
	 * @fn void* getExceptionVector()
	 * @brief Get pointer to exception vector
	 */
	void* getExceptionVector();

	/**
	 * @struct ExceptionContext
	 * @brief Context of Exception handler
	 */
	struct ExceptionContext {
		uint64_t x0;
		uint64_t x1;
		uint64_t x2;
		uint64_t x3;
		uint64_t x4;
		uint64_t x5;
		uint64_t x6;
		uint64_t x7;
		uint64_t x8;
		uint64_t x9;
		uint64_t x10;
		uint64_t x11;
		uint64_t x12;
		uint64_t x13;
		uint64_t x14;
		uint64_t x15;
		uint64_t x16;
		uint64_t x17;
		uint64_t x18;
		uint64_t x19;
		uint64_t x20;
		uint64_t x21;
		uint64_t x22;
		uint64_t x23;
		uint64_t x24;
		uint64_t x25;
		uint64_t x26;
		uint64_t x27;
		uint64_t x28;
		uint64_t x29;
		uint64_t x30;
		uint64_t sp_el0;
		uint64_t elr_el1;
		uint64_t spsr_el1;
	} __attribute__((packed));

} /* namespace */

#endif /* ifndef _INC_KERNEL_IRQ_EXCEPTION_VECTOR_H_ */
