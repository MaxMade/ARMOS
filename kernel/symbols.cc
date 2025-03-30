#include <kernel/symbols.h>

int __map_section_dummy __attribute__ ((section ("map")));

bool Symbols::init(void* symbol_map) {
	hdr = reinterpret_cast<header*>(symbol_map);
	return hdr->magic == MAGIC;
}

lib::pair<const char*, size_t> Symbols::lookup(void* addr) const {
	symbol* sym = reinterpret_cast<symbol*>(reinterpret_cast<uintptr_t>(hdr + hdr->offSymbol));
	const char* str = reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(hdr) + hdr->offString);

	for (size_t i = 0; i < hdr->numEntries; i++) {
		if (reinterpret_cast<uintptr_t>(addr) < sym[i].value)
			continue;

		if (reinterpret_cast<uintptr_t>(addr) >= sym[i].value + sym[i].size)
			continue;

		return lib::pair(&str[sym[i].idx], reinterpret_cast<uintptr_t>(addr) - sym[i].value);
	}

	return lib::pair(nullptr, 0);
}

lib::pair<void*, size_t> Symbols::getRange() const {
    void* start = hdr;
    size_t size = hdr->size;
	return lib::pair(start, size);
}
