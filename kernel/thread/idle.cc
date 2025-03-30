#include <kernel/thread/idle.h>
#include <kernel/cpu.h>

extern "C" void thread::idle() {
	while(1)
		CPU::halt();
}
