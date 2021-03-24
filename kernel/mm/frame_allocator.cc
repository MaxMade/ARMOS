#include <cerrno.h>
#include <cstdint.h>
#include <kernel/mm/frame_allocator.h>

using namespace mm;

FrameAllocator::FrameAllocator() {}

void FrameAllocator::init() {
	head = reinterpret_cast<FrameLink*>(&frames);

	for (size_t i = 0; i < NUM_FRAMES - 1; i++) {
		auto entry = reinterpret_cast<FrameLink*>(&frames[i * PAGESIZE]);
		entry->next = reinterpret_cast<FrameLink*>(&frames[(i + 1) * PAGESIZE]);
	}

	auto entry = reinterpret_cast<FrameLink*>(&frames[PAGESIZE * (NUM_FRAMES - 1)]);
	entry->next = nullptr;
}

void* FrameAllocator::earlyAlloc() {
	if (head == nullptr)
		return nullptr;

	auto ret = head;
	head = head->next;

	return ret;
}

int FrameAllocator::earlyFree(void *page) {
	if (reinterpret_cast<uintptr_t>(page) < reinterpret_cast<uintptr_t>(frames))
		return -EINVAL;

	if (reinterpret_cast<uintptr_t>(page) >= reinterpret_cast<uintptr_t>(frames) + SIZE)
		return -EINVAL;

	if (reinterpret_cast<uintptr_t>(page) % PAGESIZE != 0)
		return -EINVAL;

	auto entry = reinterpret_cast<FrameLink*>(page);
	entry->next = head;

	head = entry;

	return 0;
}

void* FrameAllocator::alloc() {
	lock.lock();
	void* ret = earlyAlloc();
	lock.unlock();
	return ret;
}

int FrameAllocator::free(void* page) {
	lock.lock();
	int ret = earlyFree(page);
	lock.unlock();
	return ret;
}
