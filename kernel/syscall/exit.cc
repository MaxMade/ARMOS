#include <unistd.h>
#include <cerrno.h>
#include <ostream.h>
#include <kernel/cpu.h>
#include <driver/cpu.h>
#include <driver/drivers.h>
#include <kernel/syscall/exit.h>
#include <kernel/syscall/syscall.h>

void syscall::exit(int status) {
	/* Disable interrupts */
	CPU::disableInterrupts();

	/* Print exit message */
	lib::panic panic;
	panic << "\n\n\rExit status: " << status;

	/* Send halt IPIs */
	for (size_t i = 0; i < driver::cpus.numCPUs(); i++) {
		if (i == CPU::getProcessorID())
			continue;
		driver::ipi.sendIPI(i, driver::IPI::IPI_MSG::HALT);
	}

	/* Halt forever */
	while(1)
		CPU::halt();
}

void syscall::__exit(irq::ExceptionContext* irq) {
	/* Get values */
	auto status = syscall::getSyscallArg<0, int>(irq);
	
	exit(status);
}
