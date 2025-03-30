#ifndef _INC_KERNEL_IRQ_SYSCALL_H_
#define _INC_KERNEL_IRQ_SYSCALL_H_

#include <utility.h>
#include <kernel/cpu_local.h>
#include <kernel/irq/exception_handler.h>
#include <kernel/irq/generic_sync_handler.h>

/**
 * @file kernel/irq/pagefault.h
 * @brief Syscall Handler
 */

namespace irq {

	/**
	 * @class SyscallHandler
	 * @brief Syscall Handler
	 */
	class SyscallHandler : public GenericSyncHandler {
		private:

			/**
			 * @var __ecs
			 * @brief Handled exception classes
			 */
			hw::reg::ESR::ExceptionClass __ecs[2] = {
				hw::reg::ESR::ExceptionClass::TRAPPED_SVC_AARCH32,
				hw::reg::ESR::ExceptionClass::TRAPPED_SVC_AARCH64,
			};

			/**
			 * @var savedContext
			 * @brief Saved context of prologue
			 */
			cpu_local<irq::ExceptionContext*> savedContext;

			/**
			 * @var NUM_HANDLERS
			 * @brief Maximum number of system call handlers
			 */
			static const size_t NUM_HANDLERS = 255;

			/**
			 * @var handlers
			 * @brief Registered system calls handlers
			 */
			void (*handlers[NUM_HANDLERS])(irq::ExceptionContext*);

		public:

			/**
			 * @fn SyscallHandler()
			 * @brief Prepare pagefault handler
			 */
			SyscallHandler();

			/**
			 * @fn int prologue(irq::ExceptionContext* context) override
			 * @brief Exception prologue
			 * @return
			 *
			 *	-  0 - Epilogue needed
			 *	-  0 - No Epilogue needed
			 *	- <0 - Error (errno)
			 */
			int prologue(irq::ExceptionContext* context) override;

			/**
			 * @fn int epilogue() override
			 * @brief Exception epilogue
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Error (errno)
			 */
			int epilogue() override;
	};

} /* namespace irq */

#endif /* ifndef _INC_KERNEL_IRQ_SYSCALL_H_ */
