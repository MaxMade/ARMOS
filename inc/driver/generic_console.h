#ifndef _INC_DRIVER_GENERIC_CONSOLE_H_
#define _INC_DRIVER_GENERIC_CONSOLE_H_

#include <cstddef.h>
#include <driver/config.h>
#include <driver/generic_driver.h>

/**
 * @file driver/generic_console.h
 * @brief Generic Base of console drivers
 */

namespace driver {

	/**
	 * @class generic_console
	 * @brief Generic console
	 */
	class generic_console : public generic_driver {
		public:
			/**
			 * @fn int init(const config& conf)
			 * @brief Intialize console
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int init(const config& conf);

			/**
			 * @fn void write(const char* buf, size_t len)
			 * @brief Write buffer to console
			 */
			void write(const char* buf, size_t len);

			/**
			 * @fn char read()
			 * @brief Read a single byte
			 */
			char read();

			/**
			 * @fn lib::pair<void*, size_t> getConfigSpace() const
			 * @brief Get used address range
			 */
			lib::pair<void*, size_t> getConfigSpace() const;
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_GENERIC_CONSOLE_H_ */
