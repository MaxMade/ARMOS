#include <cstdlib.h>
#include <kernel/BuddyAlloc.h>
#include <kernel/lock/spinlock.h>

/* Allocator */
static char mem[1 << 27];
static BuddyAllocator<(1 << 5), (1 << 27)> allocator(mem, 1 << 27);

/* Allocator lock */
static lock::spinlock allocLock;

void *lib::malloc(size_t size) {
	allocLock.lock();
	void *ret = allocator.allocate(size);
	allocLock.unlock();
	return ret;
}

void lib::free(void *ptr) {
	allocLock.lock();
	allocator.deallocate(ptr);
	allocLock.unlock();
}

void *lib::calloc(size_t nmemb, size_t size) {
	allocLock.lock();
	void *ret = allocator.allocate(nmemb * size);
	if (ret != nullptr) {
		lib::memset(ret, 0, nmemb * size);
	}
	allocLock.unlock();
	return ret;
}
