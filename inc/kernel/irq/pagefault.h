#ifndef _INC_KERNEL_IRQ_PAGE_FAULT_H_
#define _INC_KERNEL_IRQ_PAGE_FAULT_H_

#include "kernel/irq/exception_handler.h"
#include <hw/register/esr.h>
#include <kernel/irq/generic_sync_handler.h>

/**
 * @file kernel/irq/pagefault.h
 * @brief Pagefault Handler
 */

namespace irq {

	/**
	 * @class PagefaultHandler
	 * @brief Pagefault Handler
	 */
	class PagefaultHandler : public GenericSyncHandler {
		private:

			/**
			 * @var __ecs
			 * @brief Handled exception classes
			 */
			hw::reg::ESR::ExceptionClass __ecs[4] = {
				hw::reg::ESR::ExceptionClass::INSTRUCTION_ABORT,
				hw::reg::ESR::ExceptionClass::INSTRUCTION_ABORT_WITH_EL_CHANGE,
				hw::reg::ESR::ExceptionClass::DATA_ABORT,
				hw::reg::ESR::ExceptionClass::DATA_ABORT_WITH_EL_CHANGE
			};

			/**
			 * @enum Actor
			 * @brief Actor of pagefault
			 */
			enum class Actor {
				USER, /**< User Space */
				KERNEL, /**< Kernel Space */
			};

			/**
			 * @enum Operation
			 * @brief Operation class
			 */
			enum class Operation {
				READ, /**< Read Operation */
				WRITE, /**< Write Operation */
				EXECUTE, /**< Instruction Fetch Operation */
			};

			/**
			 * @enum Cause
			 * @brief Cause of pagefault
			 */
			enum class Cause {
				NON_PRESENT, /**< Non-Present Page */
				PERMISSION, /**< Inaccessible Page */
				OTHER, /**< Other Reason */
			};

			/**
			 * @fn int prologue_data_abort(irq::ExceptionContext* context)
			 * @brief Prologue for Data Abort
			 */
			int prologue_data_abort(irq::ExceptionContext* context);

			/**
			 * @fn int prologue_data_abort_elc(irq::ExceptionContext* context)
			 * @brief Prologue for Data Abort with Exception Level Change
			 */
			int prologue_data_abort_elc(irq::ExceptionContext* context);

			/**
			 * @fn int prologue_instr_abort(irq::ExceptionContext* context)
			 * @brief Prologue for Instruction Abort
			 */
			int prologue_instr_abort(irq::ExceptionContext* context);

			/**
			 * @fn int prologue_instr_abort_elc(irq::ExceptionContext* context)
			 * @brief Prologue for Instruction Abort with Exception Level Change
			 */
			int prologue_instr_abort_elc(irq::ExceptionContext* context);

			/**
			 * @fn void panic(Actor actor, Operation operation, Cause cause, irq::ExceptionContext* context)
			 * @brief Specialized Panic for dumping all pagefault related information
			 */
			void panic(Actor actor, Operation operation, Cause cause, irq::ExceptionContext* context);

		public:

			/**
			 * @fn PagefaultHandler()
			 * @brief Prepare pagefault handler
			 */
			PagefaultHandler();

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

#endif /* ifndef _INC_KERNEL_IRQ_PAGE_FAULT_H_ */
