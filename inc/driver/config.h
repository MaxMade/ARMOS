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
			 * @var configSpace
			 * @brief Configuration space of device
			 */
			lib::pair<void*,size_t> configSpace;

			/**
			 * @var intSpace
			 * @brief Configuration space of interrupts
			 */
			lib::pair<void*,size_t> intSpace;

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
			 * @fn void setInterruptRange(void* addr, size_t size)
			 * @brief Set range of interrupt setting
			 */
			void setInterruptRange(void* addr, size_t size);

			/**
			 * @fn lib::pair<void*, size_t> getInterruptRange() const
			 * @brief Get range of (device specific) interrupt setting
			 */
			lib::pair<void*, size_t> getInterruptRange() const;

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
