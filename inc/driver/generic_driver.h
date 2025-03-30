#ifndef _INC_DRIVER_GENERIC_DRIVER_H_
#define _INC_DRIVER_GENERIC_DRIVER_H_

/**
 * @file driver/generic_driver.h
 * @brief Generic base driver
 */

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

		public:
			/**
			 * @fn const char* getName() const
			 * @brief Get name of driver
			 */
			const char* getName() const;
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_GENERIC_DRIVER_H_ */
