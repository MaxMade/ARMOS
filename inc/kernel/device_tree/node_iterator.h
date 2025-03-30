#ifndef _INC_KERNEL_DEVICE_TREE_NODE_ITERATOR_H_
#define _INC_KERNEL_DEVICE_TREE_NODE_ITERATOR_H_ value

#include <cstdint.h>
#include <kernel/device_tree/node.h>

/**
 * @file kernel/device_tree/node_iterator.h
 * @brief Iterator for nodes within device tree
 */

namespace DeviceTree {

	/**
	 * @class NodeIt
	 * @brief Iterator for nodes within device tree
	 */
	class NodeIt {
		private:
			/**
			 * @var ptr
			 * @brief Pointer to FDT_BEGIN_NODE tag of associated node
			 */
			uint32_t* ptr;

			/**
			 * @var nodeBlock
			 * @brief Pointer to nodes block of device tree
			 */
			void* nodeBlock;

			/**
			 * @var stringBlock
			 * @brief Pointer to string block of device tree
			 */
			const char *stringBlock;

		public:
			/**
			 * @fn NodeIt(void *ptr, void* nodeBlock, const char* stringBlock)
			 * @brief Constructor node iterator
			 * @param ptr Pointer to current node (to FDT_PROP)
			 * @param nodeBlock Pointer to node block
			 * @param stringBlock Pointer to string block
			 */
			NodeIt(void *ptr, void* nodeBlock, const char* stringBlock);

			/**
			 * @fn NodeIt& operator++()
			 * @brief Increment iterator
			 */
			NodeIt& operator++();

			/**
			 * @fn NodeIt operator++(int)
			 * @brief Increment iterator
			 */
			NodeIt operator++(int);

			/**
			 * @fn DeviceTree::Node operator*() const
			 * @brief Create new Node from iterator
			 */
			DeviceTree::Node operator*() const;

			/**
			 * @fn bool operator==(const NodeIt& other) const
			 * @brief Compare two iteratotrs
			 */
			bool operator==(const NodeIt& other) const;

			/**
			 * @fn bool operator==(const NodeIt& other) const
			 * @brief Compare two iteratotrs
			 */
			bool operator!=(const NodeIt& other) const;
	};

} /* namespace Devicetree */

#endif /* ifndef _INC_KERNEL_DEVICE_TREE_NODE_ITERATOR_H_ */
