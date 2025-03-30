#ifndef _INC_DRIVER_GENERIC_IPI_H_
#define _INC_DRIVER_GENERIC_IPI_H_

#include <cstddef.h>
#include <functional.h>
#include <driver/config.h>
#include <driver/generic_driver.h>

/**
 * @file driver/generic_ipi.h
 * @brief Generic Base of IPI drivers
 */

namespace driver {

	/**
	 * @class generic_ipi
	 * @brief Generic IPI
	 */
	class generic_ipi : public generic_driver {
		public:
			/**
			 * @fn int init(const config& conf)
			 * @brief Intialize IPI driver
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int init(const config& conf);

			/**
			 * @fn int sendIPI(size_t cpuID, size_t msg)
			 * @brief Send IPI to cpu
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int sendIPI(size_t cpuID, size_t msg);

			/**
			 * @fn int registerHandler(size_t msg, lib::function<int()> handler)
			 * @brief Register Handler for specific value
			 */
			int registerHandler(size_t msg, lib::function<int()> handler);

			/**
			 * @fn lib::pair<void*, size_t> getConfigSpace() const
			 * @brief Get used address range
			 */
			lib::pair<void*, size_t> getConfigSpace() const;
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_GENERIC_IPI_H_ */
