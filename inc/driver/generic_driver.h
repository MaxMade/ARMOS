#ifndef _INC_DRIVER_GENERIC_DRIVER_H_
#define _INC_DRIVER_GENERIC_DRIVER_H_

/**
 * @file driver/generic_driver.h
 * @brief Generic base driver
 */

#include <utility.h>

namespace driver {

	/**
	 * @class generic_driver
	 * @brief Generic base driver
	 */
	class generic_driver {
		protected:
			/**
			 * @var name
			 * @brief Name of the driver
			 */
			const char* name;

			/**
			 * @var driverIdx
			 * @brief Index of current driver
			 */
			size_t driverIdx;

			/**
			 * @var driverNum
			 * @brief Number of registered drivers
			 */
			static size_t driverNum;

		public:
			/**
			 * @fn generic_driver
			 * @brief Construct generic driver
			 */
			generic_driver();

			generic_driver(const generic_driver& other) = delete;

			generic_driver(generic_driver&& other) = delete;

			/**
			 * @fn const char* getName() const
			 * @brief Get name of driver
			 */
			const char* getName() const;

			/**
			 * @fn static size_t getNumDrivers()
			 * @brief Get number of registered drivers
			 */
			static size_t getNumDrivers();

			/**
			 * @fn size_t getIndexDriver() const
			 * @brief Get index of driver
			 */
			size_t getIndexDriver() const;

			/**
			 * @fn virtual int prologue()
			 * @brief Exception prologue
			 * @return
			 *
			 *	-  1 - Epilogue is needed
			 *	-  0 - Epilogue isn't needed
			 *	- <0 - Error (errno)
			 */
			virtual int prologue() = 0;

			/**
			 * @fn virtual int epilogue()
			 * @brief Exception epilogue
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Error (errno)
			 */
			virtual int epilogue() = 0;
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_GENERIC_DRIVER_H_ */
