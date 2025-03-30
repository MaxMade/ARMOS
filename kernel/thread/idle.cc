#include <cerrno.h>
#include <cstdlib.h>
#include <kernel/cpu.h>
#include <kernel/thread/idle.h>

using namespace thread;

extern "C" void thread::idle() {
	while(1)
		CPU::halt();
}

IdleThreads::IdleThreads() {
	for (size_t i = 0; i < MAX_NUM_CPUS; i++) {
		threads[i].init(-1, &stacks[i][0], nullptr, true, (void*) thread::idle, nullptr);
	}
}

Context& IdleThreads::get() {
	return threads[CPU::getProcessorID()];
}
