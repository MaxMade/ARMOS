#ifndef _INC_DRIVER_CONFIG_H_
#define _INC_DRIVER_CONFIG_H_

#include <utility.h>
#include <cstddef.h>

/**
 * @file driver/config.h
 * @brief Configuration settings of a driver
 */

namespace driver {

	/**
	 * @class config
	 * @brief Configuration settings of a diver
	 */
	class config {
		private:
			/**
			 * @var addr
			 * @brief Start address of configuration space
			 */
			void* addr;

			/**
			 * @var size
			 * @brief Size of configuration space
			 */
			size_t size;

			/**
			 * @var valid
			 * @brief Check validity
			 */
			bool valid;

		public:
			/**
			 * @fn config()
			 * @brief Construct empty config
			 */
			config(bool isValid=true);

			/**
			 * @fn void setRange(void* addr, size_t size)
			 * @brief Set range of configuration space
			 */
			void setRange(void* addr, size_t size);

			/**
			 * @fn lib::pair<void*, size_t> getRange() const
			 * @brief Get range of configuration space
			 */
			lib::pair<void*, size_t> getRange() const;

			/**
			 * @fn bool isValid() const
			 * @brief Check if valid
			 */
			bool isValid() const;

			/**
			 * @fn operator bool() const
			 * @brief Check if valid
			 */
			operator bool() const;
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_CONFIG_H_ */
