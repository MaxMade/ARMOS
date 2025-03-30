#include "kernel/math.h"
#include <hw/register/tcr.h>

using namespace hw::reg;

void TCR::read() {
	asm("mrs %0, TCR_EL1" : "=r"(value.value));
}

void TCR::write() const {
	asm("msr TCR_EL1, %0" :: "r"(value.value));
}

TCR::TCR() {
	read();
}

void TCR::reset() {
	value.value = 0;
	write();
}

size_t TCR::getTTBR0RegionSize() const {
	return 1 << (64 - value.t0sz);
}

void TCR::setTTBR0RegionSize(size_t size) {
	auto t0sz = math::ld(size);
	t0sz = 64 - t0sz;
	value.t0sz = t0sz;
	write();
}

bool TCR::isTTBR0TableWalkDisabled() const {
	return value.epd0 == 1;
}

void TCR::setTTBR0TableWalkDisabled(bool disabled) {
	value.epd0 = disabled ? 1 : 0;
	write();
}

TCR::cachebility TCR::getTTBR0InnerCach() const {
	return static_cast<cachebility>(value.irgn0);
}

void TCR::setTTBR0InnerCach(cachebility val) {
	value.irgn0 = val;
	write();
}

TCR::cachebility TCR::getTTBR0OuterCach() const {
	return static_cast<cachebility>(value.orgn0);
}

void TCR::setTTBR0OuterCach(cachebility val) {
	value.orgn0 = val;
	write();
}

TCR::sharibility TCR::getTTBR0Sharibility() const {
	return static_cast<sharibility>(value.sh0);
}

void TCR::setTTBR0Sharibility(sharibility val) {
	value.sh0 = val;
	write();
}

TCR::granuleSize TCR::getTTBR0GranuleSize() const {
	if (value.tg0 == 0b00)
		return size_4k;

	if (value.tg0 == 0b10)
		return size_16k;

	return size_64k;
}

void TCR::setTTBR0GranuleSize(granuleSize val) {
	if (val == size_4k)
		value.tg0 = 0b00;

	if (val == size_16k)
		value.tg0 = 0b10;

	if (val == size_64k)
		value.tg0 = 0b01;

	write();
}

size_t TCR::getTTBR1RegionSize() const {
	return 1 << (64 - value.t0sz);
}

void TCR::setTTBR1RegionSize(size_t size) {
	auto t1sz = math::ld(size);
	t1sz = 64 - t1sz;
	value.t1sz = t1sz;
	write();
}

bool TCR::isTTBR1TableWalkDisabled() const {
	return value.epd1 == 1;
}

void TCR::setTTBR1TableWalkDisabled(bool disabled) {
	value.epd1 = disabled ? 1 : 0;
	write();
}

TCR::cachebility TCR::getTTBR1InnerCach() const {
	return static_cast<cachebility>(value.irgn1);
}

void TCR::setTTBR1InnerCach(cachebility val) {
	value.irgn1 = val;
	write();
}

TCR::cachebility TCR::getTTBR1OuterCach() const {
	return static_cast<cachebility>(value.orgn1);
}

void TCR::setTTBR1OuterCach(cachebility val) {
	value.orgn1 = val;
	write();
}


TCR::sharibility TCR::getTTBR1Sharibility() const {
	return static_cast<sharibility>(value.sh1);
}

void TCR::setTTBR1Sharibility(sharibility val) {
	value.sh1 = val;
	write();
}

TCR::granuleSize TCR::getTTBR1GranuleSize() const {
	if (value.tg1 == 0b10)
		return size_4k;

	if (value.tg1 == 0b01)
		return size_16k;

	return size_64k;
}

void TCR::setTTBR1GranuleSize(granuleSize val) {
	if (val == size_4k)
		value.tg1 = 0b10;

	if (val == size_16k)
		value.tg1 = 0b01;

	if (val == size_64k)
		value.tg1 = 0b11;

	write();
}

void TCR::useDefaultSetting() {
	reset();

	setTTBR0RegionSize(1LL << 48);
	setTTBR1RegionSize(1L << 48);

	setTTBR0GranuleSize(size_4k);
	setTTBR1GranuleSize(size_4k);
}
