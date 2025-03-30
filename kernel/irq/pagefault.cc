#include <cassert.h>
#include <cerrno.h>
#include <ostream.h>
#include <hw/register/esr.h>
#include <hw/register/far.h>
#include <kernel/error.h>
#include <kernel/debug/panic.h>
#include <kernel/irq/pagefault.h>
#include <kernel/irq/exception_handler.h>

using namespace irq;

PagefaultHandler::PagefaultHandler() {
	ecs = __ecs;
	num_ecs = 4;
}

void PagefaultHandler::panic(Actor actor, Operation operation, Cause cause, irq::ExceptionContext* context) {
	lib::panic panic;
	panic << "PANIC: ";

	/* Dump actor */
	if (actor == Actor::KERNEL)
		panic << "Kernel ";
	else
		panic << "User ";

	panic << "tries to ";

	/* Dump operation */
	if (operation == Operation::READ)
		panic << "read ";
	else if (operation == Operation::WRITE)
		panic << "write ";
	else
		panic << "execute ";

	/* Dump cause */
	if (cause == Cause::NON_PRESENT)
		panic << "a non-present page ";
	else if (cause == Cause::PERMISSION)
		panic << "a protected page ";
	else
		panic << "invalid page ";

	/* Dump faulty address */
	hw::reg::FAR far;
	panic << "(" << far.getValue() << ")";

	panic << "\n\r";

	debug::panic::generateFromIRQ("Unrecoverable Pagefault", context);
}

int PagefaultHandler::prologue_data_abort(irq::ExceptionContext* context) {
	using namespace hw::reg;

	/* Get current ESR and parse it */
	ESR esr;
	ESR::DataAbort dataAbort;
	auto ret = esr.parse_data_abort(dataAbort);
	assert(!isError(ret));
	assert(esr.getEC() == ESR::ExceptionClass::DATA_ABORT);

	/* Determine actor */
	Actor actor = Actor::KERNEL;

	/* Determine operation */
	Operation operation = dataAbort.wnr == ESR::WNR::READ ? Operation::READ : Operation::WRITE;

	/* Determine cause */
	Cause cause = Cause::OTHER;
	if (dataAbort.dfsc == ESR::DFSC::TRANSLATION_FAULT_LVL_0 ||
			dataAbort.dfsc == ESR::DFSC::TRANSLATION_FAULT_LVL_1 ||
			dataAbort.dfsc == ESR::DFSC::TRANSLATION_FAULT_LVL_2 ||
			dataAbort.dfsc == ESR::DFSC::TRANSLATION_FAULT_LVL_3) {


		cause = Cause::NON_PRESENT;

	} else if (dataAbort.dfsc == ESR::DFSC::PERMISSION_FAULT_0 ||
			dataAbort.dfsc == ESR::DFSC::PERMISSION_FAULT_1 ||
			dataAbort.dfsc == ESR::DFSC::PERMISSION_FAULT_2 ||
			dataAbort.dfsc == ESR::DFSC::PERMISSION_FAULT_3) {

		cause = Cause::PERMISSION;
	}

	/* PANIC */
	panic(actor, operation, cause, context);
	return -EINVAL;
}

int PagefaultHandler::prologue_data_abort_elc(irq::ExceptionContext* context) {
	using namespace hw::reg;

	/* Get current ESR and parse it */
	ESR esr;
	ESR::DataAbort dataAbort;
	auto ret = esr.parse_data_abort_with_el_change(dataAbort);
	assert(!isError(ret));
	assert(esr.getEC() == ESR::ExceptionClass::DATA_ABORT_WITH_EL_CHANGE);

	/* Determine actor */
	Actor actor = Actor::USER;

	/* Determine operation */
	Operation operation = dataAbort.wnr == ESR::WNR::READ ? Operation::READ : Operation::WRITE;

	/* Determine cause */
	Cause cause = Cause::OTHER;
	if (dataAbort.dfsc == ESR::DFSC::TRANSLATION_FAULT_LVL_0 ||
			dataAbort.dfsc == ESR::DFSC::TRANSLATION_FAULT_LVL_1 ||
			dataAbort.dfsc == ESR::DFSC::TRANSLATION_FAULT_LVL_2 ||
			dataAbort.dfsc == ESR::DFSC::TRANSLATION_FAULT_LVL_3) {


		cause = Cause::NON_PRESENT;

	} else if (dataAbort.dfsc == ESR::DFSC::PERMISSION_FAULT_0 ||
			dataAbort.dfsc == ESR::DFSC::PERMISSION_FAULT_1 ||
			dataAbort.dfsc == ESR::DFSC::PERMISSION_FAULT_2 ||
			dataAbort.dfsc == ESR::DFSC::PERMISSION_FAULT_3) {

		cause = Cause::PERMISSION;
	}

	/* PANIC */
	panic(actor, operation, cause, context);
	return -EINVAL;
}

int PagefaultHandler::prologue_instr_abort(irq::ExceptionContext* context) {
	using namespace hw::reg;

	/* Get current ESR and parse it */
	ESR esr;
	ESR::InstructionAbort instrAbort;
	auto ret = esr.parse_instruction_abort(instrAbort);
	assert(!isError(ret));
	assert(esr.getEC() == ESR::ExceptionClass::INSTRUCTION_ABORT);


	/* Determine actor */
	Actor actor = Actor::USER;

	/* Determine operation */
	Operation operation = Operation::EXECUTE;

	/* Determine cause */
	Cause cause = Cause::OTHER;
	if (instrAbort.ifsc == ESR::IFSC::TRANSLATION_FAULT_LVL_0 ||
			instrAbort.ifsc == ESR::IFSC::TRANSLATION_FAULT_LVL_1 ||
			instrAbort.ifsc == ESR::IFSC::TRANSLATION_FAULT_LVL_2 ||
			instrAbort.ifsc == ESR::IFSC::TRANSLATION_FAULT_LVL_3) {


		cause = Cause::NON_PRESENT;

	} else if (instrAbort.ifsc == ESR::IFSC::PERMISSION_FAULT_0 ||
			instrAbort.ifsc == ESR::IFSC::PERMISSION_FAULT_1 ||
			instrAbort.ifsc == ESR::IFSC::PERMISSION_FAULT_2 ||
			instrAbort.ifsc == ESR::IFSC::PERMISSION_FAULT_3) {

		cause = Cause::PERMISSION;
	}

	/* PANIC */
	panic(actor, operation, cause, context);
	return -EINVAL;
}

int PagefaultHandler::prologue_instr_abort_elc(irq::ExceptionContext* context) {
	using namespace hw::reg;

	/* Get current ESR and parse it */
	ESR esr;
	ESR::InstructionAbort instrAbort;
	auto ret = esr.parse_instruction_abort_with_el_change(instrAbort);
	assert(!isError(ret));
	assert(esr.getEC() == ESR::ExceptionClass::INSTRUCTION_ABORT_WITH_EL_CHANGE);


	/* Determine actor */
	Actor actor = Actor::USER;

	/* Determine operation */
	Operation operation = Operation::EXECUTE;

	/* Determine cause */
	Cause cause = Cause::OTHER;
	if (instrAbort.ifsc == ESR::IFSC::TRANSLATION_FAULT_LVL_0 ||
			instrAbort.ifsc == ESR::IFSC::TRANSLATION_FAULT_LVL_1 ||
			instrAbort.ifsc == ESR::IFSC::TRANSLATION_FAULT_LVL_2 ||
			instrAbort.ifsc == ESR::IFSC::TRANSLATION_FAULT_LVL_3) {


		cause = Cause::NON_PRESENT;

	} else if (instrAbort.ifsc == ESR::IFSC::PERMISSION_FAULT_0 ||
			instrAbort.ifsc == ESR::IFSC::PERMISSION_FAULT_1 ||
			instrAbort.ifsc == ESR::IFSC::PERMISSION_FAULT_2 ||
			instrAbort.ifsc == ESR::IFSC::PERMISSION_FAULT_3) {

		cause = Cause::PERMISSION;
	}

	/* PANIC */
	panic(actor, operation, cause, context);
	return -EINVAL;
}

int PagefaultHandler::prologue(irq::ExceptionContext* context) {
	using namespace hw::reg;

	/* Get current ESR */
	ESR esr;

	/* Branch to handler */
	if (esr.getEC() == ESR::ExceptionClass::INSTRUCTION_ABORT)
		return prologue_instr_abort(context);

	if (esr.getEC() == ESR::ExceptionClass::INSTRUCTION_ABORT_WITH_EL_CHANGE)
		return prologue_instr_abort_elc(context);

	if (esr.getEC() == ESR::ExceptionClass::DATA_ABORT)
		return prologue_data_abort(context);

	if (esr.getEC() == ESR::ExceptionClass::DATA_ABORT_WITH_EL_CHANGE)
		return prologue_data_abort_elc(context);

	return -EINVAL;
}

int PagefaultHandler::epilogue() {
	return -EINVAL;
}
