#include <kernel/utility.h>
#include <kernel/device_tree/property.h>
#include <kernel/device_tree/definition.h>

using namespace DeviceTree;

Property::Property() : valid(false), ptr(nullptr), stringBlock(nullptr) {}

Property::Property(void* ptr, const char* stringBlock) :
	valid(false), ptr(static_cast<uint32_t*>(ptr)), stringBlock(stringBlock) {

	if (ptr != nullptr && util::bigEndianToHost(*this->ptr) == FDT_PROP)
		valid = true;
}

bool Property::isValid() const {
	return valid;
}

Property::operator bool() const {
	return valid;
}

const char* Property::getName() const {
	auto prop = reinterpret_cast<FDTProp*>(ptr + 1);
	return &stringBlock[util::bigEndianToHost(prop->nameoff)];
}

lib::pair<void*, size_t> Property::getData() const {
	auto prop = reinterpret_cast<FDTProp*>(ptr + 1);
	void* addr = reinterpret_cast<void*>(prop->vals);
	size_t size = util::bigEndianToHost<uint32_t>(prop->len);
	return lib::pair(addr, size);
}
