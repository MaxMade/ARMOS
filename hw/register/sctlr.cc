#include <hw/register/sctlr.h>

using namespace hw::reg;

void SCTLR::read() {
	asm("mrs %0, SCTLR_EL1" : "=r"(value.value));
}

void SCTLR::write() const {
	asm("msr SCTLR_EL1, %0" :: "r"(value.value));
}

hw::reg::SCTLR::SCTLR() {
	read();
}

bool SCTLR::isMMUEnabled() const {
	return value.m == 1;
}

bool SCTLR::isAlignmentCheckEnabled() const {
	return value.a == 1;
}

bool SCTLR::isDataCachable() const {
	return value.c == 1;
}

bool SCTLR::isSP1AlignCheckEnabled() const {
	return value.sa == 1;
}

bool SCTLR::isSP0AlignCheckEnabled() const {
	return value.sa0 == 1;
}

bool SCTLR::isUserMaskAccessEnabled() const {
	return value.uma == 1;
}

bool SCTLR::isInstrCachable() const {
	return value.i == 1;
}

bool SCTLR::writePermissionImplNX() const {
	return value.wxn == 1;
}

bool SCTLR::isEL0LittleEndian() const {
	return value.e0e == 0;
}

bool SCTLR::isEL1LittleEndian() const {
	return value.ee == 0;
}

void SCTLR::setMMUEnabled(bool enabled) {
	value.m = enabled ? 1 : 0;
	write();
}

void SCTLR::setAlignmentCheckEnabled(bool enabled) {
	value.a = enabled ? 1 : 0;
	write();
}

void SCTLR::setDataCachable(bool enabled) {
	value.c = enabled ? 1 : 0;
	write();
}

void SCTLR::setSP1AlignCheckEnabled(bool enabled) {
	value.sa = enabled ? 1 : 0;
	write();
}

void SCTLR::setSP0AlignCheckEnabled(bool enabled) {
	value.sa0 = enabled ? 1 : 0;
	write();
}

void SCTLR::setUserMaskAccessEnabled(bool enabled) {
	value.uma = enabled ? 1 : 0;
	write();
}

void SCTLR::setInstrCachable(bool enabled) {
	value.i = enabled ? 1 : 0;
	write();
}

void SCTLR::setWritePermsetsionImplNX(bool enabled) {
	value.wxn = enabled ? 1 : 0;
	write();
}

void SCTLR::setEL0LittleEndian(bool enabled) {
	value.e0e = enabled ? 0 : 1;
	write();
}

void SCTLR::setEL1LittleEndian(bool enabled) {
	value.ee = enabled ? 0 : 1;
	write();
}
