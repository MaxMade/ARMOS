#include <kernel/cpu.h>
#include <hw/register/sctlr.h>

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
