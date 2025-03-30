#include <cstring.h>
#include <kernel/math.h>
#include <kernel/utility.h>
#include <kernel/device_tree/node.h>
#include <kernel/device_tree/definition.h>

using namespace DeviceTree;

#define MAX_STACK_SIZE 16

Node::Node() : valid(false), ptr(nullptr), nodeBlock(nullptr), stringBlock(nullptr) { }

Node::Node(void* ptr, void* nodeBlock, const char* stringBlock) :
	valid(false), ptr(static_cast<uint32_t*>(ptr)),
	nodeBlock(reinterpret_cast<uint32_t*>(nodeBlock)), stringBlock(stringBlock) {

	if (util::bigEndianToHost(*this->ptr) == FDT_BEGIN_NODE) {
		valid = true;
	}
}

bool Node::isValid() const {
	return valid;
}

Node::operator bool() const {
	return valid;
}

const char* Node::getName() const {
	return reinterpret_cast<const char *>(ptr + 1);
}

Node Node::getParent() const {
	int pos = 0;
	uint32_t* stack[MAX_STACK_SIZE];
	stack[pos] = nodeBlock;

	auto drag = nodeBlock;
	while (drag != ptr) {
		auto entry = util::bigEndianToHost(*drag);

		/* Case 0: New node */
		if (entry == FDT_BEGIN_NODE) {
			/* Size check for stack */
			if (pos == MAX_STACK_SIZE - 1)
				return Node();

			/* Save pointer on stack */
			stack[++pos] = drag;

			/* Move to next entry */
			drag++;
			auto name = reinterpret_cast<const char *>(drag);
			auto off = math::roundUp(strlen(name) + 1, FDT_ALIGNMENT);
			drag = reinterpret_cast<decltype(drag)>(reinterpret_cast<uintptr_t>(drag) + off);

			/* Skip content of prop */
			entry = util::bigEndianToHost(*drag);
			while (entry == FDT_PROP || entry == FDT_NOP) {
				/* Handle nops */
				if (entry == FDT_NOP) {
					/* Skip next entry (type) */
					drag++;

				/* Handle prop */
				} else {
					/* Skip next entry (type) */
					drag++;

					/* Skip property */
					auto prop = reinterpret_cast<FDTProp*>(drag);
					auto skip = math::roundUp(util::bigEndianToHost(prop->len) + sizeof(*prop), FDT_ALIGNMENT);
					drag = reinterpret_cast<decltype(drag)>(reinterpret_cast<uintptr_t>(drag) + skip);
				}

				entry = util::bigEndianToHost(*drag);
			}

		/* Case 1: End of node */
		} else if (entry == FDT_END_NODE) {
			/* Pop entry from stack */
			pos--;

			/* Move drag to next element */
			drag++;

		/* Case 2: Skip NOP */
		} else if (entry == FDT_NOP) {

			/* Move drag to next element */
			drag++;

		/* ERROR Case */
		} else {
			/* Return invalid node */
			return Node();
		}
	}

	/* Return found node */
	return Node(stack[pos], nodeBlock, stringBlock);
}

PropertyIt Node::begin() const {
	auto drag = ptr;

	/* Skip node */
	drag++;
	auto name = reinterpret_cast<const char *>(drag);
	auto off = math::roundUp(strlen(name) + 1, FDT_ALIGNMENT);
	drag = reinterpret_cast<decltype(drag)>(reinterpret_cast<uintptr_t>(drag) + off);

	/* Skip nops */
	while (util::bigEndianToHost(*drag) == FDT_NOP)
		drag++;

	/* Sanity check */
	if (util::bigEndianToHost(*drag) == FDT_PROP)
		return PropertyIt(drag, stringBlock);

	return PropertyIt(nullptr, stringBlock);
}

PropertyIt Node::end() const {
	return PropertyIt(nullptr, stringBlock);
}

Property Node::findProperty(const char* name) const {
	/* Iterate over properties */
	for (auto it = begin(); it != end(); ++it) {
		auto property = *it;

		/* Skip invalid properties */
		if (!property.isValid())
			continue;

		/* Check for matching properties */
		if (strcmp(name, property.getName()) == 0)
			return Property(property);
	}

	/* Return invalid property */
	return Property();
}

lib::pair<const char*, uint32_t> Node::findStringProperty(const char* name) const {
	auto prop = findProperty(name);
	if (!prop.isValid())
		return lib::pair(nullptr, 0);

	auto data = prop.getData();
	return lib::pair(reinterpret_cast<const char*>(data.first), static_cast<uint32_t>(data.second));
}

lib::pair<const char*, uint32_t> Node::findStringlistProperty(const char* name) const {
	auto prop = findProperty(name);
	if (!prop.isValid())
		return lib::pair(nullptr, 0);

	auto data = prop.getData();
	return lib::pair(reinterpret_cast<const char*>(data.first), static_cast<uint32_t>(data.second));
}

lib::pair<uint32_t, uint32_t> Node::findIntegerProperty(const char* name) const {
	auto prop = findProperty(name);
	if (!prop.isValid())
		return lib::pair(0, 0);

	/* Sanity check */
	auto data = prop.getData();
	if (data.second != 4)
		return lib::pair(0, 0);

	return lib::pair(util::bigEndianToHost(*reinterpret_cast<uint32_t*>(data.first)), 4);
}

lib::pair<RegisterIterator, RegisterIterator> Node::findRegisterProperty(const char* name) const {
	/* Get property with matching name */
	auto prop = findProperty(name);
	if (!prop.isValid())
		return lib::pair(RegisterIterator(), RegisterIterator());
	auto data = prop.getData();

	/* Set to default values */
	size_t addr = 1;
	size_t size = 1;

	/* Lookup #address-cells & #size-cells in parent node */
	auto parent = getParent();
	if (parent.isValid()) {

		/* Get #address-cells for current node */
		auto addrCells = parent.findIntegerProperty("#address-cells");

		/* If no #address-cells given, assume default value of 2 */
		if (!(addrCells.first == 0 && addrCells.second == 0))
			addr = addrCells.first;

		/* Get #size-cells for current node */
		auto sizeCells = parent.findIntegerProperty("#size-cells");

		/* If no #size-cells given, assume default value of 1 */
		if (!(sizeCells.first == 0 && sizeCells.second == 0))
			size = sizeCells.first;
	}

	/* Constructor iterators */
	RegisterIterator begin(data.first, static_cast<const uint32_t*>(data.first), data.second, addr, size);
	RegisterIterator end(nullptr, static_cast<const uint32_t*>(data.first), data.second, addr, size);

	return lib::pair(begin, end);
}
