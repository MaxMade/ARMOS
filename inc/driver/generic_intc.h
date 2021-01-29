#ifndef _INC_DRIVER_GENERIC_INTERRUPT_CONTROLLER_H_
#define _INC_DRIVER_GENERIC_INTERRUPT_CONTROLLER_H_

#include <cstddef.h>
#include <functional.h>
#include <driver/config.h>
#include <driver/generic_driver.h>

/**
 * @file driver/generic_intc.h
 * @brief Generic base for interrupt controller drivers
 */

namespace driver {

	/**
	 * @class generic_intc
	 * @brief Generic interrupt controller
	 */
	class generic_intc : public generic_driver {
		public:
			/**
			 * @fn int init(const config& conf)
			 * @brief Intialize interrupt controller
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int init(const config& conf);

			/**
			 * @fn int registerHandler(void* data, size_t size, lib::function<int()> handler)
			 * @brief Register handler for driver specific configuration
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int registerHandler(void* data, size_t size, lib::function<int()> handler);

			/**
			 * @fn int handleIRQ()
			 * @brief Handle pending irq
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int handleIRQ();
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_GENERIC_INTERRUPT_CONTROLLER_H_ */
