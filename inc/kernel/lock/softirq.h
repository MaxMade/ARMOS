#ifndef _INC_KERNEL_LOCK_SOFTIRQ_H_
#define _INC_KERNEL_LOCK_SOFTIRQ_H_

/**
 * @file kernel/lock/softirq.h
 * @brief Exception Synchronazation Mechanism
 */

#include <atomic.h>
#include <cstdlib.h>
#include <kernel/cpu_local.h>
#include <driver/generic_driver.h>

namespace lock {

	/**
	 * @class Softirq
	 * @brief Softirq
	 */
	class Softirq {
		private:
			/**
			 * @var numDrivers
			 * @brief Number of registered drivers
			 */
			size_t numDrivers;

			/**
			 * @var numCPUs
			 * @brief Number of supported CPUs
			 */
			size_t numCPUs;

			/**
			 * @var drivers
			 * @brief Array of postponed drivers
			 */
			driver::generic_driver*** drivers;

			/**
			 * @var pendingDrivers
			 * @brief Number of pending epilogue
			 */
			cpu_local<size_t> pendingDrivers;

			/**
			 * @var used
			 * @brief Usage state of softirq
			 */
			cpu_local<lib::atomic_flag> used;

		public:
			/**
			 * @fn int init()
			 * @brief Initialize softirq
			 * @warning This function must be called after all drivers are initialized
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int init();

			/**
			 * @fn int execute(driver::generic_driver* driver)
			 * @brief Execute prologue and epilogue of driver
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int execute(driver::generic_driver* driver);
	};

	/**
	 * @var softirq
	 * @brief Global softirq
	 */
	extern Softirq softirq;

} /* namespace lock */

#endif /* ifndef _INC_KERNEL_LOCK_SOFTIRQ_H_ */
