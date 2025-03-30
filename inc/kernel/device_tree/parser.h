#ifndef _INC_KERNEL_DEVICE_TREE_PARSER_H_
#define _INC_KERNEL_DEVICE_TREE_PARSER_H_

#include <tuple.h>
#include <utility.h>
#include <cstddef.h>
#include <kernel/device_tree/node_iterator.h>

/**
 * @file kernel/device_tree/parser.h
 * @brief Parser of device tree
 */

namespace DeviceTree {

	/**
	 * @class Parser
	 * @brief Abstraction of device tree parser
	 */
	class Parser {
		private:
			/**
			 * @var void* ptr
			 * @brief Pointer to raw data
			 */
			void* ptr;

			/**
			 * @var bool valid
			 * @brief Valid flag for rawData
			 */
			bool valid;

		public:
			/**
			 * @fn explicit Parser(void* rawData)
			 * @brief Constructor of Parser
			 * @param rawData Pointer to raw data
			 */
			explicit Parser(void* rawData);

			/**
			 * @fn bool isValid() const
			 * @brief Evaluate valid flag
			 */
			bool isValid() const;

			/**
			 * @fn operator bool() const
			 * @brief Evaluate valid flag
			 */
			operator bool() const;

			/**
			 * @fn NodeIt begin() const
			 * @brief Iterator to beginning node
			 */
			NodeIt begin() const;

			/**
			 * @fn NodeIt begin() const
			 * @brief Iterator to position after end node
			 */
			NodeIt end() const;
	};

} /* namespace DeviceTree */

#endif /* ifndef _INC_KERNEL_DEVICE_TREE_PARSER_H_ */
