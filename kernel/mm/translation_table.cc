#include <cerrno.h>
#include <climits.h>
#include <kernel/cpu.h>
#include <kernel/error.h>
#include <kernel/mm/translation_table.h>

using namespace mm;

TranslationTable::TranslationTable() {
	this->addr = nullptr;
}

TranslationTable::TranslationTable(void* addr) {
	this->addr = static_cast<tt_format_t*>(addr);
}

bool TranslationTable::isValid() const {
	if (addr == nullptr)
		return false;

	return reinterpret_cast<uintptr_t>(addr) % PAGESIZE == 0;
}

void* TranslationTable::getFrame() const {
	return reinterpret_cast<void*>(addr);
}

int TranslationTable::updateTTBR0() const {
	if (!isValid())
		return -EINVAL;

	CPU::setTranslationTable(addr);
	return 0;
}

void TranslationTable::reset() {
	for (size_t i = 0; i < NUM_ENTRIES; i++) {
		addr[i].value = 0;
	}
}

int TranslationTable::reset(size_t entry) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	addr[entry].value = 0;
	return 0;
}

void TranslationTable::setDefault() {
	for (size_t i = 0; i < NUM_ENTRIES; i++) {
		addr[i].value = 0;
		addr[i].one = 1;
		addr[i].attrIndex = 1;
		addr[i].ap = 1;
		addr[i].aF = 1;
	}
}

int TranslationTable::setDefault(size_t entry) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	addr[entry].value = 0;
	addr[entry].one = 1;
	addr[entry].attrIndex = 1;
	addr[entry].ap = 1;
	addr[entry].aF = 1;
	return 0;
}

int TranslationTable::getPresentBit(size_t entry) const {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	return addr[entry].present;
}

int TranslationTable::setPresentBit(size_t entry, bool present) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	addr[entry].present = present ? 1 : 0;
	return 0;
}

int TranslationTable::getAttrIndex(size_t entry) const {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	return addr[entry].attrIndex;
}

int TranslationTable::setAttrIndex(size_t entry, size_t index) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	if (index >= 8)
		return -EINVAL;

	addr[entry].attrIndex = index;
	return 0;
}

int TranslationTable::getNonSecureBit(size_t entry) const {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	return addr[entry].ns;
}

int TranslationTable::setNonSecureBit(size_t entry, bool non_secure) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	addr[entry].ns = non_secure ? 1 : 0;
	return 0;
}

int TranslationTable::getAccessControl(size_t entry) const {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	return addr[entry].ap;
}

int TranslationTable::setAccessControl(size_t entry, access_t access) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	addr[entry].ap = access;
	return 0;
}

int TranslationTable::getShareability(size_t entry) const {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	return addr[entry].sh;
}

int TranslationTable::setShareability(size_t entry, shareability_t shareability) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;


	addr[entry].sh = shareability;
	return 0;
}

int TranslationTable::getAccessedBit(size_t entry) const {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	return addr[entry].aF;
}

int TranslationTable::setAccessedBit(size_t entry, bool accessed) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	addr[entry].aF = accessed ? 1 : 0;
	return 0;
}

int TranslationTable::getNotGlobalBit(size_t entry) const {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	return addr[entry].nG;
}

int TranslationTable::setNotGlobalBit(size_t entry, bool not_global) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	addr[entry].nG = not_global ? 1 : 0;
	return 0;
}

int TranslationTable::getExecuteNeverBitKernel(size_t entry) const {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	return addr[entry].pxn;
}

int TranslationTable::setExecuteNeverBitKernel(size_t entry, bool execute_never) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	addr[entry].pxn = execute_never ? 1 : 0;
	return 0;
}

int TranslationTable::getExecuteNeverBitUser(size_t entry) const {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	return addr[entry].uxn;
}

int TranslationTable::setExecuteNeverBitUser(size_t entry, bool execute_never) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	addr[entry].uxn = execute_never ? 1 : 0;
	return 0;
}

void* TranslationTable::getAddress(size_t entry) const {
	if (entry >= NUM_ENTRIES)
		return makeError<void*>(-EINVAL);

	return reinterpret_cast<void*>(addr[entry].addr * PAGESIZE);
}

int TranslationTable::setAddress(size_t entry, void* addr) {
	if (entry >= NUM_ENTRIES)
		return -EINVAL;

	if (reinterpret_cast<uintptr_t>(addr) % PAGESIZE != 0)
		return -EINVAL;

	this->addr[entry].addr = reinterpret_cast<uintptr_t>(addr) / PAGESIZE;
	return 0;
}
