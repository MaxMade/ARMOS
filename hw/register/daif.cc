#include <hw/register/daif.h>

using namespace hw::reg;

void DAIF::read() {
	asm("mrs %0, DAIF" : "=r"(value.value));
}

void DAIF::write() const {
	asm("msr DAIF, %0" :: "r"(value.value));
}

hw::reg::DAIF::DAIF() {
	read();
}

bool DAIF::isDebugMasked() const {
	return value.debug == 1;
}

bool DAIF::isAsyncMasked() const {
	return value.async == 1;
}

bool DAIF::isIrqMasked() const {
	return value.irq == 1;
}

bool DAIF::isFiqMasked() const {
	return value.fiq == 1;
}

void DAIF::setDebugMasked(bool masked) {
	value.debug = masked ? 1 : 0;
	write();
}

void DAIF::setAsyncMasked(bool masked) {
	value.async = masked ? 1 : 0;
	write();
}

void DAIF::setIrqMasked(bool masked) {
	value.irq = masked ? 1 : 0;
	write();
}

void DAIF::setFiqMasked(bool masked) {
	value.fiq = masked ? 1 : 0;
	write();
}
