#include <climits.h>
#include <kernel/math.h>
#include <kernel/mm/paging.h>
#include <kernel/syscall/syscall.h>

bool syscall::isReadable(const void* buf, size_t size) {
	/* Get start and stop */
	auto start = (uintptr_t) buf;
	auto stop = start + size;
	start = math::roundDown(start, PAGESIZE);
	stop = math::roundUp(stop, PAGESIZE);

	/* Check pagewise if readable */
	for (auto i = start; i < stop; i += PAGESIZE) {
		if (!mm::Paging::isReadableUser(reinterpret_cast<void*>(i)))
			return false;
	}

	return true;
}
