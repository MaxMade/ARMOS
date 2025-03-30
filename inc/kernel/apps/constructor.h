#ifndef _INC_KERNEL_APPS
#define _INC_KERNEL_APPS_CONSTRUCTOR_H_ value

/**
 * @file kernel/apps/constructor.h
 * @brief Calling constructors and main
 */

namespace apps {

	/**
	 * @fn int callConstructorAndMain()
	 * @brief Call constructors and main
	 * @warning This function is currently unstable
	 */
	int callConstructorAndMain() __attribute__((section(".app.text")));

} /* namespace apps */

#endif /* ifndef _INC_KERNEL_APPS */
