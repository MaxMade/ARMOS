#ifndef _INC_DRIVER_DRIVERS_H_
#define _INC_DRIVER_DRIVERS_H_

#include <driver/generic_console.h>
#include <driver/generic_intc.h>
#include <driver/generic_timer.h>
#include <driver/generic_ipi.h>

#include <driver/arm_pl011.h>
#include <driver/mini_uart.h>
#include <driver/bcm_intc.h>
#include <driver/system_timer.h>
#include <driver/mailbox.h>

/**
 * @file driver/drivers.h
 * @brief Configruation for currently used drivers
 */

namespace driver {

/*********************************
 * Setting of the system console *
 *********************************/

/* Use arm_pl011
 * Defined in driver/arm_pl011.h
 */
#if defined(CONFIG_CONSOLE_ARM_PL011)
	using Console = arm_pl011;

/* Use mini uart
 * Defined in driver/mini_uart.h
 */
#elif defined(CONFIG_CONSOLE_BCM_MINI_UART)
	using Console = mini_uart;

/* No valid choice for console
 * Use (invalid) generic driver
 */
#else
	using Console = generic_console;
#endif

/***************************************
 * Setting of the interrupt controller *
 ***************************************/


/* Use broadcom interrupt controller
 * Defiend in driver/bcm_intc.h
 */
#if defined(CONFIG_INTC_BCM_INTC)
	using Intc = bcm_intc;

/* No valid choice for interrupt controller
 * Use (invalid) generic driver
 */
#else
	using Intc = generic_intc;
#endif

/************************
 * Setting of the timer *
 ************************/

/* Use broadcom system timer
 * Defiend in driver/system_timer.h
 */
#if defined(CONFIG_INTC_BCM_SYS_TIMER)
	using Timer = system_timer;

/* No valid choice for timer
 * Use (invalid) generic driver
 */
#else
	using Timer = generic_timer;
#endif

/**********************
 * Setting of the IPI *
 **********************/

/* Use broadcom mailbox
 * Defiend in driver/mailbox.h
 */
#if defined(CONFIG_IPI_BCM_MAILBOX)
	using IPI = mailbox;

/* No valid choice for IPI device
 * Use (invalid) generic driver
 */
#else
	using IPI = generic_ipi;
#endif

/***************************
 * Global driver instances *
 ***************************/

/**
 * @var console
 * @brief Console driver
 */
extern driver::Console console;

/**
 * @var intc
 * @brief Interrupt controller driver
 */
extern driver::Intc intc;

/**
 * @var intc
 * @brief Interrupt controller driver
 */
extern driver::Timer timer;

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_DRIVERS_H_ */
