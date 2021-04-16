#include <cerrno.h>
#include <kernel/cpu.h>
#include <kernel/thread/scheduler.h>
#include <driver/generic_ipi.h>

using namespace driver;

int generic_ipi::init(const config& conf) {
	(void) conf;
	return -ENXIO;
}

int generic_ipi::sendIPI(size_t cpuID, IPI_MSG msg) {
	(void) cpuID;
	(void) msg;

	return -ENXIO;
}

int generic_ipi::registerHandler(IPI_MSG msg, int (*handler)()) {
	(void) msg;
	(void) handler;

	return -ENXIO;
}

int generic_ipi::haltCallback() {
	CPU::disableInterrupts();
	while(1)
		CPU::halt();
	return 0;
}

int generic_ipi::reschduleCallback() {
	thread::scheduler.schedule();
	return 0;
}

int generic_ipi::panicCallback() {
	CPU::disableInterrupts();
	while (1)
		CPU::halt();

	return 0;
}
