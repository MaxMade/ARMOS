#include <kernel/utility.h>
#include <kernel/device_tree/register_iterator.h>

using namespace DeviceTree;

RegisterIterator::RegisterIterator() {
	valid = false;
	ptr = nullptr;
	data = nullptr;
	len = 0;
	addrCells = 0;
	sizeCells = 0;
}

RegisterIterator::RegisterIterator(void* ptr, const uint32_t* data, uint32_t len, uint32_t addrCells, uint32_t sizeCells) {
	this->valid = true;
	this->ptr = static_cast<uint32_t*>(ptr);
	this->data = data;
	this->len = len;
	this->addrCells = addrCells;
	this->sizeCells = sizeCells;

	if ((len % (addrCells + sizeCells)) != 0) {
		ptr = nullptr;
		valid = false;
	}
}

bool RegisterIterator::isValid() const {
	return valid;
}

RegisterIterator::operator bool() const {
	return valid;
}

RegisterIterator& RegisterIterator::operator++() {
	/* Use next entry */
	ptr += addrCells + sizeCells;

	/* Check end was reached */
	if (reinterpret_cast<uintptr_t>(ptr) >= (reinterpret_cast<uintptr_t>(data) + len))
		ptr = nullptr;

	return *this;
}

lib::pair<void*, size_t> RegisterIterator::operator*() const {
	if (!valid || ptr == nullptr)
		return lib::pair(nullptr, 0);

	uintptr_t addr = 0;
	for (size_t i = 0; i < addrCells; i++) {
		addr = addr << 32;
		addr = addr | util::bigEndianToHost(ptr[i]);
	}

	uintptr_t size = 0;
	for (size_t i = 0; i < sizeCells; i++) {
		size = size << 32;
		size = size | util::bigEndianToHost(ptr[addrCells + i]);
	}

	return lib::pair(reinterpret_cast<void*>(addr), size);
}

RegisterIterator RegisterIterator::operator++(int) {
	auto ret(*this);
	++(*this);
	return ret;
}

bool RegisterIterator::operator==(const RegisterIterator& other) const {
	return ptr == other.ptr;
}

bool RegisterIterator::operator!=(const RegisterIterator& other) const {
	return ptr != other.ptr;
}
