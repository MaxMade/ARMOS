#include <cerrno.h>
#include <hw/register/esr.h>

using namespace hw::reg;

void ESR::read() {
	asm("mrs %0, ESR_EL1" : "=r"(value.value));
}

hw::reg::ESR::ESR() {
	read();
}

uint32_t ESR::getISS() const {
	return value.iss;
}

uint32_t ESR::getIL() const {
	return value.il;
}

uint32_t ESR::getEC() const {
	return value.ec;
}

const char* ESR::getECString() const {
	if (value.ec == UNKOWN_RESON)
		return "Unknown reason";

	if (value.ec == TRAPPED_WFI_WFE)
		return "Trapped WFI or WFE instruction execution";

	if (value.ec == TRAPPED_MCR_MRC_COPROC_1111)
		return "Trapped MCR or MRC access (coproc==0b1111)";

	if (value.ec == TRAPPED_MCRR_MRRC_COPROC_1111)
		return "Trapped MCRR or MRRC access (coproc==0b1111)";

	if (value.ec == TRAPPED_MCR_MRC_COPROC_1110)
		return "Trapped MCR or MRC access (coproc==0b1110)";

	if (value.ec == TRAPPED_LDC_STC)
		return "Trapped LDC or STC access";

	if (value.ec == TRAPPED_FP)
		return "Trapped access to SVE, Advanced SIMD, or floating-point functionality";

	if (value.ec == TRAPPED_LD64B_ST64B_ST64BV_ST64BV0)
		return "Trapped execution of an LD64B, ST64B, ST64BV, or ST64BV0 instruction";

	if (value.ec == TRAPPED_MRRC_COPROC_1110)
		return "Trapped MRRC access (coproc==0b1110)";

	if (value.ec == BRANCH_TARGET_EXCEPTION)
		return "Branch target exception";

	if (value.ec == ILLEGAL_EXECUTION_STATE)
		return "Illegal Execution state";

	if (value.ec == TRAPPED_SVC_AARCH32)
		return "SVC instruction execution in AArch32 state";

	if (value.ec == TRAPPED_SVC_AARCH64)
		return "SVC instruction execution in AArch64 state";

	if (value.ec == TRAPPED_MSR_MRS_SYSTEM_INSTR_AARCH64)
		return "Trapped MSR, MRS or System instruction execution in AArch64 state";

	if (value.ec == TRAPPED_SVE)
		return "Access to SVE functionality trapped as a result of CPACR_EL1.ZEN";

	if (value.ec == TRAPPED_PAC)
		return "Exception from a Pointer Authentication instruction authentication failure";

	if (value.ec == INSTRUCTION_ABORT_WITH_EL_CHANGE)
		return "Instruction Abort from a lower Exception level (Pagefault during instruction access)";

	if (value.ec == INSTRUCTION_ABORT)
		return "Instruction Abort taken without a change in Exception level (Pagefault during instruction access)";

	if (value.ec == PC_ALIGNMENT_FAULT)
		return "PC alignment fault exception";

	if (value.ec == DATA_ABORT_WITH_EL_CHANGE)
		return "Data Abort from a lower Exception level (Pagefault during data access)";

	if (value.ec == DATA_ABORT)
		return "Data Abort taken without a change in Exception level (Pagefault during data access)";

	if (value.ec == SP_ALIGNMENT_FAULT)
		return "SP alignment fault exception";

	if (value.ec == TRAPPED_FP_AARCH32)
		return "Trapped floating-point exception taken from AArch32 state";

	if (value.ec == TRAPPED_FP_AARCH64)
		return "Trapped floating-point exception taken from AArch64 state";

	if (value.ec == SERROR_INTERRUPT)
		return "SError interrupt";

	if (value.ec == BREAKPOINT_WITH_EL_CHANGE)
		return "Breakpoint exception from a lower Exception level";

	if (value.ec == BREAKPOINT)
		return "Breakpoint exception taken without a change in Exception level";

	if (value.ec == SW_STEP_EXCEPTION_WITH_EL_CHANGE)
		return "Software Step exception from a lower Exception level";

	if (value.ec == SW_STEP_EXCEPTION)
		return "Software Step exception taken without a change in Exception level";

	if (value.ec == WATCHPOINT_WITH_EL_CHANGE)
		return "Watchpoint exception from a lower Exception level";

	if (value.ec == WATCHPOINT)
		return "Watchpoint exception taken without a change in Exception level";

	if (value.ec == BKPT_AARCH32)
		return "BKPT instruction execution in AArch32 state";

	if (value.ec == BRK_AARCH64)
		return "BRK instruction execution in AArch64 state";

	return nullptr;
}

int ESR::parse_data_abort_with_el_change(DataAbort& dataAbort) const {
	if (value.ec != DATA_ABORT_WITH_EL_CHANGE)
		return -EINVAL;

	dataAbort.value = value.iss;

	return 0;
}

int ESR::parse_data_abort(DataAbort& dataAbort) const {
	if (value.ec != DATA_ABORT)
		return -EINVAL;

	dataAbort.value = value.iss;

	return 0;
}

int ESR::parse_instruction_abort_with_el_change(InstructionAbort& instrAbort) const {
	if (value.ec != INSTRUCTION_ABORT_WITH_EL_CHANGE)
		return -EINVAL;

	instrAbort.value = value.iss;

	return 0;
}

int ESR::parse_instruction_abort(InstructionAbort& instrAbort) const {
	if (value.ec != INSTRUCTION_ABORT)
		return -EINVAL;

	instrAbort.value = value.iss;

	return 0;
}

int ESR::parse_watchpoint_with_el_change(Watchpoint& watchpoint) const {
	if (value.ec != WATCHPOINT_WITH_EL_CHANGE)
		return -EINVAL;

	watchpoint.value = value.iss;

	return 0;
}

int ESR::parse_watchpoint(Watchpoint& watchpoint) const {
	if (value.ec != WATCHPOINT)
		return -EINVAL;

	watchpoint.value = value.iss;

	return 0;
}
