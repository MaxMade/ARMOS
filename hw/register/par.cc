#include <hw/register/par.h>

using namespace hw::reg;

PAR::PAR() {
	asm("mrs %0, PAR_EL1" : "=r"(value));
}

bool PAR::isValid() const {
	ParValid par;
	par.value = value;

	return par.f == F::SUCCCESS;
}

PAR::operator bool() const {
	return isValid();
}

PAR::ParValid PAR::getValid() const {
	ParValid par;
	par.value = value;
	return par;
}

PAR::ParInvalid PAR::getInvalid() const {
	ParInvalid par;
	par.value = value;
	return par;
}
