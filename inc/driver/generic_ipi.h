#ifndef _INC_DRIVER_GENERIC_IPI_H_
#define _INC_DRIVER_GENERIC_IPI_H_

#include <cstddef.h>
#include <cstdint.h>
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
			 * @enum IPI_MSG
			 * @brief IPI Message
			 */
			enum class IPI_MSG : uint32_t {
				PANIC = (1 << 0),      /**< Panic Broadcast */
				RESCHEDULE = (1 << 1), /**< Panic Broadcast */
			};

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
			 * @fn int sendIPI(size_t cpuID, IPI_MSG msg)
			 * @brief Send IPI to cpu
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int sendIPI(size_t cpuID, IPI_MSG msg);

			/**
			 * @fn int registerHandler(IPI_MSG msg, lib::function<int()> handler)
			 * @brief Register Handler for specific value
			 */
			int registerHandler(IPI_MSG msg, lib::function<int()> handler);
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_GENERIC_IPI_H_ */
