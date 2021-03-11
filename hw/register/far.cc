#include <hw/register/far.h>

using namespace hw::reg;

FAR::FAR() {
	asm("mrs %0, FAR_EL1" : "=r"(val));
}

void* FAR::getValue() const {
	return reinterpret_cast<void*>(val);
}

FAR::operator uintptr_t() const {
	return val;
}

FAR::operator void*() const {
	return reinterpret_cast<void*>(val);
}
