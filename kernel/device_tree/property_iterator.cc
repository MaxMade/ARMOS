#include <kernel/math.h>
#include <kernel/utility.h>
#include <kernel/device_tree/definition.h>
#include <kernel/device_tree/property_iterator.h>

using namespace DeviceTree;

PropertyIt::PropertyIt(void* ptr, const char* stringBlock) :
	ptr(static_cast<uint32_t*>(ptr)), stringBlock(static_cast<const char*>(stringBlock)) { }

PropertyIt& PropertyIt::operator++() {
	auto prop = reinterpret_cast<FDTProp*>(ptr + 1);

	/* Prepare offset */
	auto off = sizeof(*prop) + util::bigEndianToHost(prop->len);

	/* Move to next property or end (nullptr) */
	auto tmp = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(prop) + off);
	tmp = reinterpret_cast<uint32_t*>(math::roundUp(reinterpret_cast<uintptr_t>(tmp), FDT_ALIGNMENT));

	/* Skip NOPs */
	while (util::bigEndianToHost(*tmp) == FDT_NOP)
		tmp++;

	/* Check for new beginning property */
	if (util::bigEndianToHost(*tmp) == FDT_PROP)
		ptr = tmp;
	else
		ptr = nullptr;

	return *this;
}

PropertyIt PropertyIt::operator++(int) {
	auto ret(*this);
	++(*this);
	return ret;
}

bool PropertyIt::operator==(const PropertyIt& other) const {
	return this->ptr == other.ptr;
}

bool PropertyIt::operator!=(const PropertyIt& other) const {
	return this->ptr != other.ptr;
}


Property PropertyIt::operator*() const {
	return Property(static_cast<void*>(ptr), stringBlock);
}
