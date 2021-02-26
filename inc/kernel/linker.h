#ifndef _INC_KERNEL_LINKER_H_
#define _INC_KERNEL_LINKER_H_

/**
 * @file kernel/linker.h
 * @brief Access to linker information
 */

#include <utility.h>

namespace linker {

	/**
	 * @fn lib::pair<void*, size_t> getTextSegment()
	 * @brief Get (page-aligned) start address and size of Text segment
	 */
	lib::pair<void*, size_t> getTextSegment();

	/**
	 * @fn lib::pair<void*, size_t> getRODataSegment()
	 * @brief Get (page-aligned) start address and size of ROData segment
	 */
	lib::pair<void*, size_t> getRODataSegment();

	/**
	 * @fn lib::pair<void*, size_t> getDataSegment()
	 * @brief Get (page-aligned) start address and size of Data segment
	 */
	lib::pair<void*, size_t> getDataSegment();

	/**
	 * @fn lib::pair<void*, size_t> getBSSSegment()
	 * @brief Get (page-aligned) start address and size of BSS segment
	 */
	lib::pair<void*, size_t> getBSSSegment();

	/**
	 * @fn lib::pair<void*, size_t> getAppTextSegment()
	 * @brief Get (page-aligned) start address and size of Text app segment
	 */
	lib::pair<void*, size_t> getAppTextSegment();

	/**
	 * @fn lib::pair<void*, size_t> getAppRODataSegment()
	 * @brief Get (page-aligned) start address and size of ROData app segment
	 */
	lib::pair<void*, size_t> getAppRODataSegment();

	/**
	 * @fn lib::pair<void*, size_t> getAppDataSegment()
	 * @brief Get (page-aligned) start address and size of Data app segment
	 */
	lib::pair<void*, size_t> getAppDataSegment();

	/**
	 * @fn lib::pair<void*, size_t> getAppBSSSegment()
	 * @brief Get (page-aligned) start address and size of BSS app segment
	 */
	lib::pair<void*, size_t> getAppBSSSegment();

	/**
	 * @fn void* getSymbolMapStart()
	 * @brief Get (page-aligned) start address of the symbol map
	 */
	void* getSymbolMapStart();
}

#endif /* ifndef _INC_KERNEL_LINKER_H_ */
