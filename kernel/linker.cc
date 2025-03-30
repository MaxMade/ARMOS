#include <cstdint.h>
#include <kernel/linker.h>

extern uintptr_t __TEXT_START;
extern uintptr_t __TEXT_END;

extern uintptr_t __RODATA_START;
extern uintptr_t __RODATA_END;

extern uintptr_t __DATA_START;
extern uintptr_t __DATA_END;

extern uintptr_t __BSS_START;
extern uintptr_t __BSS_END;

extern uintptr_t __APP_TEXT_START;
extern uintptr_t __APP_TEXT_END;

extern uintptr_t __APP_RODATA_START;
extern uintptr_t __APP_RODATA_END;

extern uintptr_t __APP_DATA_START;
extern uintptr_t __APP_DATA_END;

extern uintptr_t __APP_BSS_START;
extern uintptr_t __APP_BSS_END;

extern uintptr_t __MAP_START;

lib::pair<void*, size_t> linker::getTextSegment() {
	void* start = &__TEXT_START;
	void* end = &__TEXT_END;
	size_t size = reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(start);

	return lib::pair(start, size);
}

lib::pair<void*, size_t> linker::getRODataSegment() {
	void* start = &__RODATA_START;
	void* end = &__RODATA_END;
	size_t size = reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(start);

	return lib::pair(start, size);
}

lib::pair<void*, size_t> linker::getDataSegment() {
	void* start = &__DATA_START;
	void* end = &__DATA_END;
	size_t size = reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(start);

	return lib::pair(start, size);
}

lib::pair<void*, size_t> linker::getBSSSegment() {
	void* start = &__BSS_START;
	void* end = &__BSS_END;
	size_t size = reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(start);

	return lib::pair(start, size);
}

lib::pair<void*, size_t> linker::getAppTextSegment() {
	void* start = &__APP_TEXT_START;
	void* end = &__APP_TEXT_END;
	size_t size = reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(start);

	return lib::pair(start, size);
}

lib::pair<void*, size_t> linker::getAppRODataSegment() {
	void* start = &__APP_RODATA_START;
	void* end = &__APP_RODATA_END;
	size_t size = reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(start);

	return lib::pair(start, size);
}

lib::pair<void*, size_t> linker::getAppDataSegment() {
	void* start = &__APP_DATA_START;
	void* end = &__APP_DATA_END;
	size_t size = reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(start);

	return lib::pair(start, size);
}

lib::pair<void*, size_t> linker::getAppBSSSegment() {
	void* start = &__APP_BSS_START;
	void* end = &__APP_BSS_END;
	size_t size = reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(start);

	return lib::pair(start, size);
}

void* linker::getSymbolMapStart() {
	void* start = &__MAP_START;
	return start;
}
