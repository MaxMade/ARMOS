#include <cerrno.h>
#include <cerrno.h>
#include <cstdlib.h>
#include <cstdint.h>
#include <kernel/cpu.h>
#include <kernel/math.h>
#include <kernel/utility.h>
#include <kernel/thread/smp.h>
#include <driver/cpu.h>
#include <driver/drivers.h>

using namespace thread;

#define STACKSIZE (2 * 1024 * 1024)
#define STACKALIGN 16

extern uintptr_t _start;
extern uintptr_t __CPU_ID;
extern uintptr_t __CPU_STACK;

SMP::SMP() {
	apps = 0;
}

int SMP::start() {
	/* Get number of CPUs */
	auto numCPUS = driver::cpus.numCPUs();
	char* stacks = reinterpret_cast<char*>(lib::malloc(numCPUS * STACKSIZE));
	if (stacks == nullptr)
		return -ENOMEM;

	/* Roundup to stack alignment */
	stacks = reinterpret_cast<char*>(math::roundDown(reinterpret_cast<uintptr_t>(stacks), STACKALIGN));


	/* Trampoline values */
	uint64_t* id = reinterpret_cast<uint64_t*>(&__CPU_ID);
	uint64_t* stack = reinterpret_cast<uint64_t*>(&__CPU_STACK);

	/* Address of counter */
	volatile size_t* counter = &apps;

	/* Save startup address */
	uint64_t startAddr = reinterpret_cast<uint64_t>(&_start);

	/* TODO: Ensure cpus.begin() is actual boot CPU */
	size_t idx = 1;
	for (auto cpu = (driver::cpus.begin() + 1); cpu != driver::cpus.end(); ++cpu) {
		/* Prepare trampoline */
		*stack = reinterpret_cast<uint64_t>(&stacks[idx * STACKSIZE - STACKALIGN]);
		*id = idx;

		/* Prepare start address for application processors */
		util::mmioWrite(reinterpret_cast<uint64_t*>(cpu->getSpintable()), startAddr);
		CPU::dataBarrier();

		/* Wake up CPUs */
		CPU::wakeup();

		/* Wait till update via registerCPU */
		while(*counter != idx);

		/* Update active CPU idx */
		idx++;
	}

	return 0;
}

void SMP::registerCPU() {
	apps++;
}

size_t SMP::getRegisteredCPUS() const {
	return apps;
}
