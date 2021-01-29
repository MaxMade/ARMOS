#include <new.h>

void* operator new(size_t size) {
	return lib::malloc(size);
}

void* operator new[](size_t size) {
	return lib::malloc(size);
}

void operator delete(void* p) {
	lib::free(p);
}

void operator delete[](void* p) {
	lib::free(p);
}

void operator delete(void* p, size_t size) {
	(void) size;
	lib::free(p);
}

void operator delete [](void* p, size_t size) {
	(void) size;
	lib::free(p);
}
