#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <__ListAllocator.h>

/* Allocator */
static const size_t ALLOCATOR_SIZE (64 * 1024 * 1024);
static char mem[ALLOCATOR_SIZE];
static ListAllocator<32, BestFit> allocator(mem, ALLOCATOR_SIZE);

/* Locking */
typedef bool lock_t;
static lock_t allocator_lock = false;

static void lock() {
	while(__atomic_test_and_set(&allocator_lock, __ATOMIC_SEQ_CST));
}

static void unlock() {
	__atomic_clear(&allocator_lock, __ATOMIC_SEQ_CST);
}


void *malloc(size_t size) {
	lock();
	auto ret = allocator.allocate(size);
	if (ret == nullptr) {
		errno = ENOMEM;
	}
	unlock();
	return ret;
}

void free(void *ptr) {
	lock();
	allocator.deallocate(ptr);
	unlock();
}

void *calloc(size_t nmemb, size_t size) {
	lock();
	auto ret = allocator.allocate(nmemb * size);
	if (ret != nullptr) {
		memset(ret, 0, nmemb * size);
	} else {
		errno = ENOMEM;
	}
	unlock();
	return ret;
}
