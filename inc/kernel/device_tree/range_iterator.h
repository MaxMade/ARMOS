#ifndef _INC_KERNEL_DEVICE_TREE_RANGE_ITERATOR_H_
#define _INC_KERNEL_DEVICE_TREE_RANGE_ITERATOR_H_

#include <cstdint.h>
#include <cstddef.h>
#include <tuple.h>

/**
 * @file kernel/device_tree/range_iterator.h
 * @brief Iterator for range triples
 */

namespace DeviceTree {

	/**
	 * @class RangeIterator
	 * @brief Iterator for register tuples
	 */
	class RangeIterator {
		private:
			/**
			 * @var valid
			 * @brief Check if iterator is valid
			 */
			bool valid;

			/**
			 * @var ptr
			 * @brief Pointer to the current tuple
			 */
			uint32_t* ptr;

			/**
			 * @var data
			 * @breif Data of the register
			 */
			const uint32_t* data;

			/**
			 * @var len
			 * @brief Length of data
			 */
			uint32_t len;

			/**
			 * @var addrCells
			 * @brief Number of uint32_t cells per address
			 */
			uint32_t addrCells;

			/**
			 * @var sizeCells
			 * @brief Number of uint32_t cells per size
			 */
			uint32_t sizeCells;

			/**
			 * @var parentAddrCells
			 * @brief Number of uint32_t cells per size
			 */
			uint32_t parentAddrCells;

		public:

			/**
			 * @fn RangeIterator()
			 * @brief Construct an invalid RangeIterator
			 */
			RangeIterator();

			/**
			 * @fn RangeIterator(void* ptr, const uint32_t* data, uint32_t len, uint32_t addrCells, uint32_t sizeCells, uint32_t parentAddrCells)
			 * @brief Construct a RangeIterator for the given parameters
			 * @brief ptr Pointer to current tuple
			 * @brief data Data of the register
			 * @brief len Length of data
			 * @brief addrCells Number address cells in child space
			 * @brief sizeCells Number size cells in child space
			 * @brief parentAddrCells Number address cells in parent space
			 */
			RangeIterator(void* ptr, const uint32_t* data, uint32_t len, uint32_t addrCells, uint32_t sizeCells, uint32_t parentAddrCells);

			/**
			 * @fn bool isValid() const
			 * @brief Check if iterator is valid
			 */
			bool isValid() const;

			/**
			 * @fn operator bool() const
			 * @brief Check if iterator is valid
			 */
			operator bool() const;

			/**
			 * @fn RangeIterator& operator++()
			 * @brief Get next entry
			 */
			RangeIterator& operator++();

			/**
			 * @fn RangeIterator operator++(int)
			 * @brief Get next entry
			 */
			RangeIterator operator++(int);

			/**
			 * @fn lib::tuple<void*, void*, size_t> operator*() const
			 * @brief Access underlying triple (child address, parent address, size)
			 */
			lib::tuple<void*, void*, size_t> operator*() const;

			/**
			 * @fn bool operator==(const RangeIterator& other) const
			 * @brief Compare two iterators
			 */
			bool operator==(const RangeIterator& other) const;

			/**
			 * @fn bool operator!=(const RangeIterator& other) const
			 * @brief Compare two iterators
			 */
			bool operator!=(const RangeIterator& other) const;
	};

} /* namespace DeviceTree */

#endif /* ifndef _INC_KERNEL_DEVICE_TREE_RANGE_ITERATOR_H_ */
