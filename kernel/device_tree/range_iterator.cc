#include <kernel/utility.h>
#include <kernel/device_tree/range_iterator.h>

using namespace DeviceTree;

RangeIterator::RangeIterator() {
	valid = false;
	ptr = nullptr;
	data = nullptr;
	len = 0;
	addrCells = 0;
	sizeCells = 0;
	parentAddrCells = 0;
}

RangeIterator::RangeIterator(void* ptr, const uint32_t* data, uint32_t len, uint32_t addrCells, uint32_t sizeCells, uint32_t parentAddrCells) {
	this->valid = true;
	this->ptr = static_cast<uint32_t*>(ptr);
	this->data = data;
	this->len = len;
	this->addrCells = addrCells;
	this->sizeCells = sizeCells;
	this->parentAddrCells = parentAddrCells;

	if ((len % (addrCells + sizeCells)) != 0) {
		ptr = nullptr;
		valid = false;
	}
}

bool RangeIterator::isValid() const {
	return valid;
}

RangeIterator::operator bool() const {
	return valid;
}

RangeIterator& RangeIterator::operator++() {
	/* Use next entry */
	ptr += addrCells + sizeCells + parentAddrCells;

	/* Check end was reached */
	if (reinterpret_cast<uintptr_t>(ptr) >= (reinterpret_cast<uintptr_t>(data) + len))
		ptr = nullptr;

	return *this;
}

lib::tuple<void*, void*, size_t> RangeIterator::operator*() const {
	if (!valid || ptr == nullptr)
		return lib::tuple<void*,void*,size_t>(nullptr, nullptr, 0);

	uintptr_t addr = 0;
	for (size_t i = 0; i < addrCells; i++) {
		addr = addr << 32;
		addr = addr | util::bigEndianToHost(ptr[i]);
	}

	uintptr_t parentAddr = 0;
	for (size_t i = 0; i < parentAddrCells; i++) {
		parentAddr = parentAddr << 32;
		parentAddr = parentAddr | util::bigEndianToHost(ptr[addrCells + i]);
	}

	uintptr_t size = 0;
	for (size_t i = 0; i < sizeCells; i++) {
		size = size << 32;
		size = size | util::bigEndianToHost(ptr[addrCells + parentAddrCells + i]);
	}

	return lib::tuple(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(parentAddr), size);
}

RangeIterator RangeIterator::operator++(int) {
	auto ret(*this);
	++(*this);
	return ret;
}

bool RangeIterator::operator==(const RangeIterator& other) const {
	return ptr == other.ptr;
}

bool RangeIterator::operator!=(const RangeIterator& other) const {
	return ptr != other.ptr;
}
