#ifndef _INC_KERNEL_DEVICE_TREE_NODE_H_
#define _INC_KERNEL_DEVICE_TREE_NODE_H_

#include <cstdint.h>
#include <kernel/device_tree/property_iterator.h>
#include <kernel/device_tree/register_iterator.h>
#include <kernel/device_tree/range_iterator.h>

/**
 * @file kernel/device_tree/node.h
 * @brief Node within device tree
 */

namespace DeviceTree {

	/**
	 * @class Node
	 * @brief Abstraction of node within device tree
	 */
	class Node {
		private:
			/**
			 * @var valid
			 * @brief Check if valid node
			 */
			bool valid;

			/**
			 * @var ptr
			 * @brief Pointer to FDT_BEGIN_NODE
			 */
			uint32_t* ptr;

			/**
			 * @var nodeBlock
			 * @brief Pointer to node block of device tree
			 */
			uint32_t* nodeBlock;
			/**
			 * @var stringBlock
			 * @brief Pointer to string block of device tree
			 */
			const char* stringBlock;

		public:
			/**
			 * @fn Node()
			 * @brief Create empty (invalid) node
			 */
			Node();

			/**
			 * @fn Node(void* ptr, void* nodeBlock, const char* stringBlock)
			 * @brief Construct Node
			 * @param ptr Poiner to FDT_BEGIN_NODE
			 * @param nodeBlock Pointer to node block
			 * @param stringBlock Pointer to string block
			 */
			Node(void* ptr, void* nodeBlock, const char* stringBlock);

			/**
			 * @fn bool isValid() const
			 * @brief Check if node is valid
			 */
			bool isValid() const;

			/**
			 * @fn operator bool() const
			 * @brief Check if node is valid
			 */
			operator bool() const;

			/**
			 * @fn const char* getName() const
			 * @brief Get name of node
			 */
			const char* getName() const;

			/**
			 * @fn Node getParent() const
			 * @brief Get parent for current node
			 * @warning parent might be invalid!
			 */
			Node getParent() const;

			/**
			 * @fn PropertyIt begin() const
			 * @brief Iterate over properties within node
			 */
			PropertyIt begin() const;

			/**
			 * @fn PropertyIt end() const
			 * @brief Iterate over properties within node
			 */
			PropertyIt end() const;

			/**
			 * @fn Property findProperty(const char* name) const
			 * @brief Find property by name
			 * @warning Returned property might be invalid
			 */
			Property findProperty(const char* name) const;

			/**
			 * @fn lib::pair<const char*, uint32_t> findStringProperty(const char* name) const
			 * @brief Find property by name and evaluate as string
			 *
			 *	<Pointer to string, Size of string in bytes> - Success
			 *	<nullptr, 0> - Failure
			 */
			lib::pair<const char*, uint32_t> findStringProperty(const char* name) const;

			/**
			 * @fn lib::pair<const char*, uint32_t> findStringlistProperty(const char* name) const
			 * @brief Find property by name and evaluate as stringlist
			 * @return
			 *
			 *	<Pointer to strings, Size of strings in bytes> - Success
			 *	<nullptr, 0> - Failure
			 */
			lib::pair<const char*, uint32_t> findStringlistProperty(const char* name) const;

			/**
			 * @fn lib::pair<uint32_t, uint32_t> findIntegerProperty(const char* name) const
			 * @brief Find property by name and evaluate as integer
			 * @return
			 *
			 *	<Integer, 4> - Success
			 *	<0, 0> - Failure
			 */
			lib::pair<uint32_t, uint32_t> findIntegerProperty(const char* name) const;

			/**
			 * @fn lib::pair<uint64_t, uint32_t> findLongProperty(const char* name) const
			 * @brief Find property by name and evaluate as long
			 * @return
			 *
			 *	<Integer, 8> - Success
			 *	<0, 0> - Failure
			 */
			lib::pair<uint32_t, uint32_t> findLongProperty(const char* name) const;

			/**
			 * @fn lib::pair<RegisterIterator, RegisterIterator> findRegisterProperty(const char* name) const
			 * @brief Find porperty by name and evaluate as list of registers (start address, size)
			 * @return pair of begin and end iterator
			 */
			lib::pair<RegisterIterator, RegisterIterator> findRegisterProperty(const char* name) const;

			/**
			 * @fn lib::pair<RangeIterator, RangeIterator> findRangeProperty(const char* name) const
			 * @brief Find porperty by name and evaluate as list of range (start address in child space, start address in parent space, size)
			 * @return pair of begin and end iterator
			 */
			lib::pair<RangeIterator, RangeIterator> findRangeProperty(const char* name) const;
	};

} /* namespace DeviceTree */

#endif /* ifndef _INC_KERNEL_DEVICE_TREE_NODE_H_ */
