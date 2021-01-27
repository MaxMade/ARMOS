#include <utility.h>
#include <cstring.h>
#include <kernel/math.h>
#include <kernel/error.h>
#include <kernel/utility.h>
#include <kernel/device_tree/parser.h>
#include <kernel/device_tree/definition.h>
#include <kernel/mm/paging.h>
#include <driver/config.h>

using namespace DeviceTree;

Parser::Parser(void* rawData) {
	ptr = rawData;
	auto hdr = reinterpret_cast<FDTHeader*>(rawData);
	size = util::bigEndianToHost(hdr->totalsize);
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

driver::config Parser::findConfig(const driver::generic_driver& driver) const {
	auto name = driver.getName();

	for (auto nodeIt = begin(); nodeIt != end(); ++nodeIt) {
		auto node = *nodeIt;
		if (!node.isValid())
			continue;

		/* Check if node has compatible property */
		auto compatible = node.findStringlistProperty("compatible");
		if (compatible.first == nullptr || compatible.second == 0)
			continue;

		/* Check if node is compatible with driver */
		if (strcmp(name, compatible.first) != 0)
			continue;

		/* Check if node has valid reg property */
		auto regIt = node.findRegisterProperty("reg");
		if (regIt.first == regIt.second)
			continue;

		/* Save range */
		auto reg = *regIt.first;
		void *addr = reg.first;
		size_t size = reg.second;

		/* Range fixup */
		auto parent = node.getParent();
		if (parent.isValid()) {
			auto range = parent.findRangeProperty("ranges");
			for (auto it = range.first; it != range.second; ++it) {
				auto parentRange = *it;
				auto childAddr = lib::get<0>(parentRange);
				auto parentAddr = lib::get<1>(parentRange);
				auto childSize = lib::get<2>(parentRange);

				if (reinterpret_cast<uintptr_t>(addr) >= reinterpret_cast<uintptr_t>(childAddr) &&
				   reinterpret_cast<uintptr_t>(addr) < reinterpret_cast<uintptr_t>(childAddr) + childSize){

					auto offset = reinterpret_cast<uintptr_t>(addr) - reinterpret_cast<uintptr_t>(childAddr);
					addr = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(parentAddr) + offset);
				}
			}
		}

		/* Prepare config */
		driver::config ret;
		ret.setRange(addr, size);

		return ret;
	}

	return driver::config(false);
}

lib::pair<void*, size_t> Parser::getConfigSpace() const {
	auto hdr = reinterpret_cast<FDTHeader*>(ptr);

	void* start = ptr;
	size_t size = util::bigEndianToHost(hdr->totalsize);

	return lib::pair(start, size);
}

int Parser::createMapping() const {
	mm::Paging paging;

	auto mapStart = math::roundDown(reinterpret_cast<uintptr_t>(ptr), PAGESIZE);
	auto mapSize = math::roundUp(size, PAGESIZE);

	for (auto addr = mapStart; addr < mapStart + mapSize; addr += PAGESIZE) {
		auto ret = paging.earlyMap(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(addr),
				mm::Paging::KERNEL_MAPPING, mm::Paging::WRITABLE, mm::Paging::DEVICE_ATTR);

		if (isError(ret))
			return castError<int, decltype(ret)>(ret);
	}

	for (auto node : *this) {
		if (!node.isValid())
			continue;

		auto rangeProp = node.findRangeProperty("ranges");
		for (auto rangeIt = rangeProp.first; rangeIt != rangeProp.second; ++rangeIt) {
			auto range = *rangeIt;
			auto mapAddr = math::roundDown(reinterpret_cast<uintptr_t>(lib::get<1>(range)), PAGESIZE);
			auto mapSize = math::roundUp(lib::get<2>(range), PAGESIZE);

			for (uintptr_t addr = mapAddr; addr < mapAddr + mapSize; addr += PAGESIZE) {
				auto ret = paging.earlyMap(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(addr),
						mm::Paging::KERNEL_MAPPING, mm::Paging::WRITABLE, mm::Paging::DEVICE_ATTR);

				if (isError(ret))
					return castError<int, decltype(ret)>(ret);
			}
		}
	}

	return 0;
}
