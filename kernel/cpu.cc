#include <kernel/cpu.h>
#include <hw/register/sctlr.h>
#include <hw/register/daif.h>

bool CPU::isBigEndian() {
	hw::reg::SCTLR sctlr;
	return !sctlr.isEL1LittleEndian();
}

bool CPU::isLittleEndian() {
	hw::reg::SCTLR sctlr;
	return sctlr.isEL1LittleEndian();
}

void CPU::loadExeptionVector(void* addr) {
	asm("msr VBAR_EL1, %0" :: "r"(addr));
}

void CPU::enableInterrupts() {
	hw::reg::DAIF daif;
	daif.setDebugMasked(false);
	daif.setAsyncMasked(false);
	daif.setIrqMasked(false);
	daif.setFiqMasked(false);
}

void CPU::disableInterrupts() {
	hw::reg::DAIF daif;
	daif.setDebugMasked(true);
	daif.setAsyncMasked(true);
	daif.setIrqMasked(true);
	daif.setFiqMasked(true);
}

bool CPU::areInterruptsEnabled() {
	hw::reg::DAIF daif;
	return !daif.isDebugMasked() || !daif.isAsyncMasked() || !daif.isIrqMasked() || !daif.isFiqMasked();
}

bool CPU::areInterruptsDisabled() {
	hw::reg::DAIF daif;
	return daif.isDebugMasked() && daif.isAsyncMasked() && daif.isIrqMasked() && daif.isFiqMasked();
}

void CPU::invalidatePage(void *vaddr) {
	asm(
		"dsb ish\n\t"
		"tlbi vaae1is, %0\n\t"
		"dsb ish\n\t"
		"isb\n\t"
		:: "r"(vaddr)
	);
}

void CPU::invalidateTLB() {
	asm(
		"dsb ish\n\t"
		"tlbi alle1is\n\t"
		"dsb ish\n\t"
		"isb\n\t"
	);
}

size_t CPU::getProcessorID() {
	uint64_t mpidr = 0;
	asm("mrs %0, MPIDR_EL1" : "=r"(mpidr));
	return (mpidr & 0xFF);
}

void CPU::setTranslationTable(void *addr) {
	asm("msr TTBR0_EL1, %0" :: "r"(addr));
}

void* CPU::getTranslationTable() {
	void* addr;
	asm("mrs %0, TTBR0_EL1" : "=r"(addr));
	return addr;
}

void CPU::halt() {
	asm("wfe");
}

void CPU::wakeup() {
	asm("sev");
}

void CPU::dataBarrier() {
	asm("dsb sy");
}
