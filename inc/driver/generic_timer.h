#ifndef _INC_DRIVER_GENERIC_TIMER_H_
#define _INC_DRIVER_GENERIC_TIMER_H_

#include <cstddef.h>
#include <functional.h>
#include <driver/config.h>
#include <driver/generic_driver.h>

/**
 * @file driver/generic_timer.h
 * @brief Generic base of timer drivers
 */

namespace driver {

	/**
	 * @class generic_timer
	 * @brief Generic timer
	 */
	class generic_timer : public generic_driver {
		public:
			/**
			 * @fn int init(const config& conf)
			 * @brief Intialize timer
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int init(const config& conf);

			/**
			 * @fn int windup(size_t ms)
			 * @brief Windup and activate timer
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int windup(size_t ms);

			/**
			 * @fn size_t interval() const
			 * @brief Get current configured interval
			 */
			size_t interval() const;

			/**
			 * @fn size_t getTicks() const
			 * @brief Get number of ticks since startup
			 */
			size_t getTicks() const;

			/**
			 * @fn int registerFunction(size_t ms, lib::function<int(void)> callback)
			 * @brief Register callback which is executed in a regular interval
			 * @warning ms must be multiple of interval
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int registerFunction(size_t ms, lib::function<int(void)> callback);
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_GENERIC_TIMER_H_ */
