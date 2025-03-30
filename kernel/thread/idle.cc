#include <cerrno.h>
#include <cstdlib.h>
#include <kernel/cpu.h>
#include <kernel/config.h>
#include <kernel/thread/idle.h>

using namespace thread;

extern "C" void thread::idle() {
	while(1)
		CPU::halt();
}

int IdleThreads::init() {
	char** stacks = (char**) new char[STACK_SIZE];
	if (stacks == nullptr)
		return -ENOMEM;

	auto& context = threads.get();
	context.init(-1, stacks, nullptr, true, (void*) thread::idle);

	return 0;
}

Context& IdleThreads::get() {
	return threads.get();
}
