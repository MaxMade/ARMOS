#include <cerrno.h>
#include <cstdlib.h>
#include <kernel/cpu.h>
#include <kernel/error.h>
#include <kernel/thread/idle.h>
#include <kernel/debug/panic.h>

using namespace thread;

extern "C" void thread::idle() {
	while(1)
		CPU::halt();
}

int IdleThreads::init() {
	for (size_t i = 0; i < MAX_NUM_CPUS; i++) {
		if (int err = threads[i].init(-1, true, (void*) thread::idle, nullptr, nullptr); isError(err))
			return err;
	}

	return 0;
}

Context& IdleThreads::get() {
	return threads[CPU::getProcessorID()];
}
