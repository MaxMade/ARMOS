#ifndef _INC_KERNEL_IRQ_SYNC_HANDLER_H_
#define _INC_KERNEL_IRQ_SYNC_HANDLER_H_

#include <kernel/irq/generic_sync_handler.h>

namespace irq {

	/**
	 * @class SyncHandler
	 * @brief Synchronous exception handler
	 */
	class SyncHandler {
		private:
			/**
			 * @var NUM_HANDLERS
			 * @brief Number of handlers
			 * @details
			 * This number is derived from the Exception Class (EC) field of
			 * the Exception Syndrome (ESR).
			 */
			static const size_t NUM_HANDLERS = (1 << 6);
			/**
			 * @var handlers
			 * @brief List of handlers
			 */
			irq::GenericSyncHandler* handlers[NUM_HANDLERS] = {0};

		public:
			/**
			 * @fn int registerHandler(GenericSyncHandler* handler)
			 * @brief Register handler for exception for exceptions
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int registerHandler(GenericSyncHandler* handler);

			/**
			 * @fn driver::generic_driver* getHandler()
			 * @brief Get Handler for pending exception
			 * @return
			 *
			 *	- Pointer to driver - Succes
			 *	- nullptr - Failure
			 */
			driver::generic_driver* getHandler();
	};

	extern SyncHandler syncHandler;

} /* namespace irq */

#endif /* ifndef _INC_KERNEL_IRQ_SYNC_HANDLER_H_ */
