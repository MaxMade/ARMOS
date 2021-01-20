#ifndef _INC_DRIVER_DRIVERS_H_
#define _INC_DRIVER_DRIVERS_H_

#include <driver/arm_pl011.h>

namespace driver {

/* Setting of the system console */

/* Use arm_pl011
 * Defined in driver/arm_pl011.h
 */
#if defined(CONFIG_CONSOLE_ARM_PL011)
	using console = arm_pl011;

/* No valid choice for console
 * Abort compiling with a error message
 */
#else
	#error "No valid setting for CONFIG_CONSOLE"
#endif

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_DRIVERS_H_ */
