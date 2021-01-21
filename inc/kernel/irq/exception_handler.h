#ifndef _INC_KERNEL_IRQ_EXCEPTION_VECTOR_H_
#define _INC_KERNEL_IRQ_EXCEPTION_VECTOR_H_ value

/**
 * @file kernel/irq/exception_handler.h
 * @brief Operations concerning the exception handlers
 */

namespace irq {

	/**
	 * @fn void* getExceptionVector()
	 * @brief Get pointer to exception vector
	 */
	void* getExceptionVector();

} /* namespace */

#endif /* ifndef _INC_KERNEL_IRQ_EXCEPTION_VECTOR_H_ */
