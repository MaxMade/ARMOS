#include <cstring.h>
#include <kernel/math.h>
#include <kernel/utility.h>
#include <kernel/device_tree/definition.h>
#include <kernel/device_tree/node_iterator.h>

using namespace DeviceTree;

NodeIt::NodeIt(void *ptr, void* nodeBlock, const char* stringBlock) :
	ptr(static_cast<uint32_t*>(ptr)), nodeBlock(static_cast<uint32_t*>(nodeBlock)), stringBlock(stringBlock) { }

bool NodeIt::operator==(const NodeIt& other) const {
	return ptr == other.ptr;
}

bool NodeIt::operator!=(const NodeIt& other) const {
	return ptr != other.ptr;
}

NodeIt& NodeIt::operator++() {
	/* Skip FDT_BEGIN_NODE */
	ptr++;

	/* Skip next entry ("<node-name>@<unit-address>") */
	auto name = reinterpret_cast<const char *>(ptr);
	auto off = math::roundUp(strlen(name) + 1, FDT_ALIGNMENT);
	ptr = reinterpret_cast<decltype(ptr)>(reinterpret_cast<uintptr_t>(ptr) + off);

	/* Skip next entries (properties, node end or nops) */
	auto drag = util::bigEndianToHost(*ptr);
	while (drag == FDT_PROP || drag == FDT_NOP || drag == FDT_END_NODE) {

		/* Skip FDT_PROP,FDT_NOP or FDT_END_NODE */
		ptr++;

		/* Handle FDT_NOP */
		if (drag == FDT_NOP) {
			drag = util::bigEndianToHost(*ptr);
			continue;

		/* Handle FDT_END_NODE */
		} else if (drag == FDT_END_NODE) {
			drag = util::bigEndianToHost(*ptr);
			continue;

		/* Handle FDT_PROP */
		} else {
			auto prop = reinterpret_cast<FDTProp*>(ptr);
			auto skip = math::roundUp(util::bigEndianToHost(prop->len) + sizeof(*prop), FDT_ALIGNMENT);
			ptr = reinterpret_cast<decltype(ptr)>(reinterpret_cast<uintptr_t>(ptr) + skip);
			drag = util::bigEndianToHost(*ptr);
		}
	}

	/* Check if end is reached */
	if (drag == FDT_END)
		ptr = nullptr;

	return *this;
}

NodeIt NodeIt::operator++(int) {
	NodeIt ret(*this);
	++(*this);
	return ret;
}

Node NodeIt::operator*() const {
	return Node(ptr, nodeBlock, stringBlock);
}
