#ifndef _INC_KERNEL_DEVICE_TREE_REGISTER_ITERATOR_H_
#define _INC_KERNEL_DEVICE_TREE_REGISTER_ITERATOR_H_

#include <utility.h>
#include <cstdint.h>
#include <cstddef.h>

/**
 * @file kernel/device_tree/register_iterator.h
 * @brief Iterator for register pairs
 */

namespace DeviceTree {

	/**
	 * @class RegisterIterator
	 * @brief Iterator for register pairs
	 */
	class RegisterIterator {
		private:
			/**
			 * @var valid
			 * @brief Check if iterator is valid
			 */
			bool valid;

			/**
			 * @var ptr
			 * @brief Pointer to the current pair
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

		public:

			/**
			 * @fn RegisterIterator()
			 * @brief Construct an invalid RegisterIterator
			 */
			RegisterIterator();

			/**
			 * @fn RegisterIterator(void* ptr, const uint32_t* data, uint32_t len, uint32_t addrCells, uint32_t sizeCells)
			 * @brief Construct a RegisterIterator for the given parameters
			 * @brief ptr Pointer to current pair
			 * @brief data Data of the register
			 * @brief len Length of data
			 * @brief addrCells Number address cells in parent space
			 * @brief sizeCells Number size cells in parent space
			 */
			RegisterIterator(void* ptr, const uint32_t* data, uint32_t len, uint32_t addrCells, uint32_t sizeCells);

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
			 * @fn RegisterIterator& operator++()
			 * @brief Get next entry
			 */
			RegisterIterator& operator++();

			/**
			 * @fn RegisterIterator operator++(int)
			 * @brief Get next entry
			 */
			RegisterIterator operator++(int);

			/**
			 * @fn lib::pair<void*, size_t> operator*() const
			 * @brief Access underlying pair (address, size)
			 */
			lib::pair<void*, size_t> operator*() const;

			/**
			 * @fn bool operator==(const RegisterIterator& other) const
			 * @brief Compare two iterators
			 */
			bool operator==(const RegisterIterator& other) const;

			/**
			 * @fn bool operator!=(const RegisterIterator& other) const
			 * @brief Compare two iterators
			 */
			bool operator!=(const RegisterIterator& other) const;
	};

} /* namespace DeviceTree */

#endif /* ifndef _INC_KERNEL_DEVICE_TREE_REGISTER_ITERATOR_H_ */
