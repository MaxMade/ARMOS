#include <cerrno.h>
#include <cstdint.h>
#include <kernel/mm/translation_table_allocator.h>

using namespace mm;

TTAllocator::TTAllocator() {}

void TTAllocator::init() {
	head = reinterpret_cast<TTPageLink*>(&pages);

	for (size_t i = 0; i < NUM_PAGES - 1; i++) {
		auto entry = reinterpret_cast<TTPageLink*>(&pages[i * PAGESIZE]);
		entry->next = reinterpret_cast<TTPageLink*>(&pages[(i + 1) * PAGESIZE]);
	}

	auto entry = reinterpret_cast<TTPageLink*>(&pages[PAGESIZE * (NUM_PAGES - 1)]);
	entry->next = nullptr;
}

void* TTAllocator::earlyAlloc() {
	if (head == nullptr)
		return nullptr;

	auto ret = head;
	head = head->next;

	return ret;
}

int TTAllocator::earlyFree(void *page) {
	if (reinterpret_cast<uintptr_t>(page) < reinterpret_cast<uintptr_t>(pages))
		return -EINVAL;

	if (reinterpret_cast<uintptr_t>(page) >= reinterpret_cast<uintptr_t>(pages) + SIZE)
		return -EINVAL;

	if (reinterpret_cast<uintptr_t>(page) % PAGESIZE != 0)
		return -EINVAL;

	auto entry = reinterpret_cast<TTPageLink*>(page);
	entry->next = head;

	head = entry;

	return 0;
}

void* TTAllocator::alloc() {
	lock.lock();
	void* ret = earlyAlloc();
	lock.unlock();
	return ret;
}

int TTAllocator::free(void* page) {
	lock.lock();
	int ret = earlyFree(page);
	lock.unlock();
	return ret;
}
