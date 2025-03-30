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
			 * @fn int registerHandler(void* data, size_t size, generic_driver* driver)
			 * @brief Register driver (prologue/epilogue) for driver specific configuration
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int registerHandler(void* data, size_t size, generic_driver* driver);

			/**
			 * @fn generic_driver* getHandler() const
			 * @brief Get handler for pending IRQ
			 * @return
			 *
			 *	- Pointer to handler      - Success
			 *	- makeError<generic_driver*>(errno) - Failure
			 */
			generic_driver* getHandler() const;
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_GENERIC_INTERRUPT_CONTROLLER_H_ */
