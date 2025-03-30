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
	if (value.ec == unkown_reson)
		return "Unknown reason";

	if (value.ec == trapped_wfi_wfe)
		return "Trapped WFI or WFE instruction execution";

	if (value.ec == trapped_mcr_mrc_0)
		return "Trapped MCR or MRC access";

	if (value.ec == trapped_mcrr_mrrc)
		return "Trapped MCRR or MRRC access";

	if (value.ec == trapped_mcr_mrc_1)
		return "Trapped MCR or MRC access";

	if (value.ec == trapped_ldc_stc)
		return "Trapped LDC or STC access";

	if (value.ec == trapped_fp)
		return "Trapped access to SVE, Advanced SIMD, or floating-point functionality";

	if (value.ec == trapped_mrrc)
		return "Trapped MRRC access";

	if (value.ec == brnach_target_exception)
		return "Branch target exception";

	if (value.ec == illegal_execution_state)
		return "Illegal Execution state";

	if (value.ec == trapped_svc_aarch32)
		return "SVC instruction execution in AArch32 state";

	if (value.ec == trapped_svc_aarch64)
		return "SVC instruction execution in AArch64 state";

	if (value.ec == trapped_msr_mrs_system_instr_aarch64)
		return "Trapped MSR, MRS or System instruction execution in AArch64 state";

	if (value.ec == trapped_sve)
		return "Access to SVE functionality trapped as a result of CPACR_EL1.ZEN";

	if (value.ec == trapped_pac)
		return "Exception from a Pointer Authentication instruction authentication failure";

	if (value.ec == instruction_abort_with_el_change)
		return "Instruction Abort from a lower Exception level (Pagefault during instruction access)";

	if (value.ec == instruction_abort)
		return "Instruction Abort taken without a change in Exception level (Pagefault during instruction access)";

	if (value.ec == pc_alignment_fault)
		return "PC alignment fault exception";

	if (value.ec == data_abort_with_el_change)
		return "Data Abort from a lower Exception level (Pagefault during data access)";

	if (value.ec == data_abort)
		return "Data Abort taken without a change in Exception level (Pagefault during data access)";

	if (value.ec == sp_alignment_fault)
		return "SP alignment fault exception";

	if (value.ec == trapped_fp_aarch32)
		return "Trapped floating-point exception taken from AArch32 state";

	if (value.ec == trapped_fp_aarch64)
		return "Trapped floating-point exception taken from AArch64 state";

	if (value.ec == serror_interrupt)
		return "SError interrupt";

	if (value.ec == breakpoint_with_el_change)
		return "Breakpoint exception from a lower Exception level";

	if (value.ec == breakpoint)
		return "Breakpoint exception taken without a change in Exception level";

	if (value.ec == sw_step_exception_with_el_change)
		return "Software Step exception from a lower Exception level";

	if (value.ec == sw_step_exception)
		return "Software Step exception taken without a change in Exception level";

	if (value.ec == watchpoint_with_el_change)
		return "Watchpoint exception from a lower Exception level";

	if (value.ec == watchpoint)
		return "Watchpoint exception taken without a change in Exception level";

	if (value.ec == bkpt_aarch32)
		return "BKPT instruction execution in AArch32 state";

	if (value.ec == brk_aarch64)
		 return "BRK instruction execution in AArch64 state";

		return nullptr;
}
