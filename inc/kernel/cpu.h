#ifndef _INC_KERNEL_CPU_H_
#define _INC_KERNEL_CPU_H_

#include <cstddef.h>

/**
 * @file kernel/cpu.h
 * @brief Common CPU operations
 */

namespace CPU {

	/**
	 * @fn bool isBigEndian()
	 * @brief Check if CPU is big endian
	 */
	bool isBigEndian();

	/**
	 * @fn bool isBigEndian()
	 * @brief Check if CPU is little endian
	 */
	bool isLittleEndian();

	/**
	 * @fn void loadExeptionVector(void* addr)
	 * @brief Load exception vector at addr
	 */
	void loadExeptionVector(void* addr);

	/**
	 * @fn void enableInterrupts()
	 * @brief Enable Debug interrupts, SErrror interrupts, IRQs and FIQs
	 */
	void enableInterrupts();

	/**
	 * @fn void disableInterrupts()
	 * @brief Disable Debug interrupts, SErrror interrupts, IRQs and FIQs
	 */
	void disableInterrupts();

	/**
	 * @fn bool areInterruptsEnabled()
	 * @brief Check if at least one of the following is enabled: Debug interrupts, SErrror interrupts, IRQs and FIQs
	 */
	bool areInterruptsEnabled();

	/**
	 * @fn bool areInterruptsEnabled()
	 * @brief Check if all of the following are disabled: Debug interrupts, SErrror interrupts, IRQs and FIQs
	 */
	bool areInterruptsDisabled();

	/**
	 * @fn void invalidatePage(void *vaddr)
	 * @brief Invalidate page
	 */
	void invalidatePage(void *vaddr);

	/**
	 * @fn void invalidateTLB()
	 * @brief invalidate whole TLB
	 */
	void invalidateTLB();

	/**
	 * @fn size_t getProcessorID()
	 * @brief Get processor ID
	 */
	size_t getProcessorID();

	/**
	 * @fn void setTranslationTable(void *addr)
	 * @brief Update TTBR0
	 */
	void setTranslationTable(void *addr);

	/**
	 * @fn void* getTranslationTable()
	 * @brief Receive current value of TTBR0
	 */
	void* getTranslationTable();

} /* namespace CPU */

#endif /* ifndef _INC_KERNEL_CPU_H_ */
