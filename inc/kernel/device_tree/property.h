#ifndef _INC_KERNEL_DEVICE_TREE_PROPERTY_H_
#define _INC_KERNEL_DEVICE_TREE_PROPERTY_H_

#include <cstdint.h>
#include <utility.h>

namespace DeviceTree {

/**
 * @class Property
 * @brief Abstraction of a property in device tree
 */
class Property {
	private:
		/**
		 * @var valid
		 * @brief Validity of property
		 */
		bool valid;

		/**
		 * @var ptr
		 * @brief Pointer to FDT_PROP
		 */
		uint32_t* ptr;

		/**
		 * @var stringBlock
		 * @brief Pointer to string block
		 */
		const char* stringBlock;

	public:
		/**
		 * @fn Property()
		 * @brief Create invalid property
		 */
		Property();

		/**
		 * @fn Property(void* ptr, const char* stringBlock)
		 * @brief Constuctor to property
		 * @param ptr Pointer to FDT_PROP
		 * @param stringBlock Pointer to string block
		 */
		Property(void* ptr, const char* stringBlock);

		/**
		 * @fn bool isValid() const
		 * @brief Check if property is valid
		 */
		bool isValid() const;

		/**
		 * @fn operator bool() const
		 * @brief Check if property is valid
		 */
		operator bool() const;

		/**
		 * @fn const char* getName() const
		 * @brief Get name of property
		 */
		const char* getName() const;

		/**
		 * @fn lib::pair<void*, size_t> getData() const
		 * @brief Get raw data of property
		 */
		lib::pair<void*, size_t> getData() const;
};

} /* namespace DeviceTree */

#endif /* ifndef _INC_KERNEL_DEVICE_TREE_PROPERTY_H_ */
