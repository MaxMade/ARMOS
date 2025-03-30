#ifndef _INC_KERNEL_CPU_H_
#define _INC_KERNEL_CPU_H_

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

} /* namespace CPU */

#endif /* ifndef _INC_KERNEL_CPU_H_ */
