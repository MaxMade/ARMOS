#ifndef _INC_KERNEL_DEVICE_TREE_PROPERTY_ITERATOR_H_
#define _INC_KERNEL_DEVICE_TREE_PROPERTY_ITERATOR_H_ value

#include <cstdint.h>
#include <kernel/device_tree/property.h>

/**
 * @file kernel/device_tree/property_iterator.h
 * @brief Iterator for properties within a node of the device tree
 */

namespace DeviceTree {

	/**
	 * @class PropertyIt
	 * @brief Property Iterator
	 */
	class PropertyIt {
		private:
			/**
			 * @var ptr
			 * @brief Pointer to FDT_PROP
			 */
			uint32_t* ptr;

			/**
			 * @var stringBlock
			 * @brief Pointer to string block of device tree
			 */
			const char* stringBlock;

		public:
			/**
			 * @fn PropertyIt(void* ptr, const char* stringBlock)
			 * @brief Constructor of property iterator
			 * @param ptr Pointer to FDT_PROP
			 * @param stringBlock Pointer to string block
			 */
			PropertyIt(void* ptr, const char* stringBlock);

			/**
			 * @fn PropertyIt& operator++()
			 * @brief Iterate to next entry
			 */
			PropertyIt& operator++();

			/**
			 * @fn PropertyIt operator++(int)
			 * @brief Iterate to next entry
			 */
			PropertyIt operator++(int);

			/**
			 * @fn Property operator*()
			 * @brief Get underlying property
			 */
			Property operator*() const;

			/**
			 * @fn bool operator==(const PropertyIt& other) const
			 * @brief Compare iterators
			 */
			bool operator==(const PropertyIt& other) const;

			/**
			 * @fn bool operator!=(const PropertyIt& other) const
			 * @brief Compare iterators
			 */
			bool operator!=(const PropertyIt& other) const;
	};


} /* namespae DeviceTree */

#endif /* ifndef _INC_KERNEL_DEVICE_TREE_PROPERTY_ITERATOR_H_ */
