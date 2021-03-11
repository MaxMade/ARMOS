#ifndef _INC_KERNEL_IRQ_GENERIC_SYNC_HANDLER_H_
#define _INC_KERNEL_IRQ_GENERIC_SYNC_HANDLER_H_

#include <type_traits.h>
#include <hw/register/esr.h>
#include <driver/generic_driver.h>

/**
 * @file kernel/irq/generic_sync_handler.h
 * @brief Generic Synchronous Exception Handler
 */

namespace irq {

	class GenericSyncHandler : public driver::generic_driver {
		protected:
			/**
			 * @var ecs
			 * @brief Pointer to list of exception classes
			 */
			hw::reg::ESR::ExceptionClass* ecs;

			/**
			 * @var num_ecs
			 * @brief Number of entries within list of exception classes
			 */
			size_t num_ecs;

		public:
			/**
			 * @typedef const_iterator
			 * @brief Iterator for supported exception classes
			 */
			using const_iterator = const hw::reg::ESR::ExceptionClass*;

			/**
			 * @fn virtual int prologue(irq::ExceptionContext* context) = 0
			 * @brief Exception prologue
			 * @return
			 *
			 *	-  1 - Epilogue is needed
			 *	-  0 - Epilogue isn't needed
			 *	- <0 - Error (errno)
			 */
			virtual int prologue(irq::ExceptionContext* context) = 0;

			/**
			 * @fn virtual int epilogue()
			 * @brief Exception epilogue
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Error (errno)
			 */
			virtual int epilogue() = 0;

			/**
			 * @fn const_iterator beginEC() const
			 * @brief Iterator to start of exception class list
			 */
			const_iterator beginEC() const;

			/**
			 * @fn const_iterator endEC() const
			 * @brief Iterator to end of exception class list
			 */
			const_iterator endEC() const;
	};

} /* namespace irq */

#endif /* ifndef _INC_KERNEL_IRQ_GENERIC_SYNC_HANDLER_H_ */
