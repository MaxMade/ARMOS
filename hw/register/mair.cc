#include <cerrno.h>
#include <hw/register/mair.h>

using namespace hw::reg;

void MAIR::read() {
	asm("mrs %0, MAIR_EL1" : "=r"(value.value));
}

void MAIR::write() const {
	asm("msr MAIR_EL1, %0" :: "r"(value.value));
}

MAIR::MAIR() {
	read();
}

int MAIR::getAttribute(size_t entry) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	return value.attrs[entry];
}

int MAIR::setDeviceAttribute(size_t entry, dev_attr_t attr) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	/* Encoding:
	 * 0b0000dd00 where dd equals attr
	 */
	value.attrs[entry] = (attr & 0b11) << 2;
	write();

	return 0;
}

int MAIR::setNormalAttribute(size_t entry, normal_attr_t inner, normal_attr_t outer) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	/* Encoding:
	 * 0booooiiii where oooo equals outer and iiii equals inner
	 */
	value.attrs[entry] = ((outer & 0b1111) << 4) | (inner & 0b1111);
	write();

	return 0;
}

void MAIR::useDefaultLayout() {
	setDeviceAttribute(0, ngnre);
	setNormalAttribute(1, nc, nc);
}
