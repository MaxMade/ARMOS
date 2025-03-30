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
