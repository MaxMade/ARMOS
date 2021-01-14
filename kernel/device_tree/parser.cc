#include "utility.h"
#include <kernel/utility.h>
#include <kernel/device_tree/parser.h>
#include <kernel/device_tree/definition.h>

using namespace DeviceTree;

Parser::Parser(void* rawData) {
	ptr = rawData;
	auto hdr = reinterpret_cast<FDTHeader*>(rawData);
	valid = util::bigEndianToHost(hdr->magic) == FDT_MAGIC;
}

bool Parser::isValid() const {
	return valid;
}

Parser::operator bool() const {
	return valid;
}

NodeIt Parser::begin() const {
	auto hdr = reinterpret_cast<FDTHeader*>(ptr);

	auto offStruct = util::bigEndianToHost(hdr->off_dt_struct);
	auto ptrStruct = reinterpret_cast<uintptr_t>(hdr) + offStruct;

	auto offStrings = util::bigEndianToHost(hdr->off_dt_strings);
	auto ptrStrings = reinterpret_cast<uintptr_t>(hdr) + offStrings;

	return NodeIt(reinterpret_cast<void*>(ptrStruct), reinterpret_cast<void*>(ptrStruct), reinterpret_cast<const char*>(ptrStrings));
}

NodeIt Parser::end() const {
	auto hdr = reinterpret_cast<FDTHeader*>(ptr);

	auto offStruct = util::bigEndianToHost(hdr->off_dt_struct);
	auto ptrStruct = reinterpret_cast<uintptr_t>(hdr) + offStruct;

	auto offStrings = util::bigEndianToHost(hdr->off_dt_strings);
	auto ptrStrings = reinterpret_cast<uintptr_t>(hdr) + offStrings;

	return NodeIt(nullptr, reinterpret_cast<void*>(ptrStruct), reinterpret_cast<const char*>(ptrStrings));
}
