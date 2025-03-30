#include "kernel/irq/exception_handler.h"
#include <ios.h>
#include <ostream.h>
#include <functional.h>
#include <driver/cpu.h>
#include <driver/drivers.h>
#include <kernel/cpu.h>
#include <kernel/debug/panic.h>
#include <kernel/debug/stack_trace.h>
#include <hw/register/general_purpose_reg.h>

int debug::panic::init() {
	auto handler = []() -> int {
		CPU::disableInterrupts();
		while (1)
			CPU::halt();

		return 0;
	};

	return driver::ipi.registerHandler(driver::IPI::IPI_MSG::PANIC, lib::function<int()>(handler));
}

void debug::panic::generate(const char msg[]) {
	lib::panic panic;

	/* Disable interrupts */
	CPU::disableInterrupts();

	/* Send IPIs */
	auto numCPUS = driver::cpus.numCPUs();
	for (size_t i = 0; i < numCPUS; i++) {
		driver::ipi.sendIPI(i, driver::IPI::IPI_MSG::PANIC);
	}

	/* Print panic message */
	panic << "PANIC: " << msg << "\n\n\r";

	/* Generate register dump */
	void* regs[31];
	x0(regs[1]);
	x1(regs[1]);
	x2(regs[2]);
	x3(regs[3]);
	x4(regs[4]);
	x5(regs[5]);
	x6(regs[6]);
	x7(regs[7]);
	x8(regs[8]);
	x9(regs[9]);
	x10(regs[10]);
	x11(regs[11]);
	x12(regs[12]);
	x13(regs[13]);
	x14(regs[14]);
	x15(regs[15]);
	x16(regs[16]);
	x17(regs[17]);
	x18(regs[18]);
	x19(regs[19]);
	x20(regs[20]);
	x21(regs[21]);
	x22(regs[22]);
	x23(regs[23]);
	x24(regs[24]);
	x25(regs[25]);
	x26(regs[26]);
	x27(regs[27]);
	x28(regs[28]);
	x29(regs[29]);
	x30(regs[30]);

	for (size_t i = 0; i <= 30; i++) {
		if (i < 10) {
			panic << "   x" << i << ": ";
		} else {
			panic << "  x" << i << ": ";
		}

		panic.width(10);
		panic << regs[i];
		panic.width(0);


		if (i % 3 == 2)
			panic << "\n\r";
	}
	panic << "\n\n\r";


	/* Generate stack trace */
	debug::stack_trace(15);

	/* Endless loop */
	while (1)
		CPU::halt();
}

void debug::panic::generateFromIRQ(const char msg[], irq::ExceptionContext* exceptionContext) {
	lib::panic panic;

	/* Disable interrupts */
	CPU::disableInterrupts();

	/* Send IPIs */
	auto numCPUS = driver::cpus.numCPUs();
	for (size_t i = 0; i < numCPUS; i++) {
		driver::ipi.sendIPI(i, driver::IPI::IPI_MSG::PANIC);
	}

	/* Print panic message */
	panic << "PANIC: " << msg << "\n\n\r";

	/* Generate register dump */
	void* regs[31];
	regs[0] = reinterpret_cast<void*>(exceptionContext->x0);
	regs[1] = reinterpret_cast<void*>(exceptionContext->x1);
	regs[2] = reinterpret_cast<void*>(exceptionContext->x2);
	regs[3] = reinterpret_cast<void*>(exceptionContext->x3);
	regs[4] = reinterpret_cast<void*>(exceptionContext->x4);
	regs[5] = reinterpret_cast<void*>(exceptionContext->x5);
	regs[6] = reinterpret_cast<void*>(exceptionContext->x6);
	regs[7] = reinterpret_cast<void*>(exceptionContext->x7);
	regs[8] = reinterpret_cast<void*>(exceptionContext->x8);
	regs[9] = reinterpret_cast<void*>(exceptionContext->x9);
	regs[10] = reinterpret_cast<void*>(exceptionContext->x10);
	regs[11] = reinterpret_cast<void*>(exceptionContext->x11);
	regs[12] = reinterpret_cast<void*>(exceptionContext->x12);
	regs[13] = reinterpret_cast<void*>(exceptionContext->x13);
	regs[14] = reinterpret_cast<void*>(exceptionContext->x14);
	regs[15] = reinterpret_cast<void*>(exceptionContext->x15);
	regs[16] = reinterpret_cast<void*>(exceptionContext->x16);
	regs[17] = reinterpret_cast<void*>(exceptionContext->x17);
	regs[18] = reinterpret_cast<void*>(exceptionContext->x18);
	regs[19] = reinterpret_cast<void*>(exceptionContext->x19);
	regs[20] = reinterpret_cast<void*>(exceptionContext->x20);
	regs[21] = reinterpret_cast<void*>(exceptionContext->x21);
	regs[22] = reinterpret_cast<void*>(exceptionContext->x22);
	regs[23] = reinterpret_cast<void*>(exceptionContext->x23);
	regs[24] = reinterpret_cast<void*>(exceptionContext->x24);
	regs[25] = reinterpret_cast<void*>(exceptionContext->x25);
	regs[26] = reinterpret_cast<void*>(exceptionContext->x26);
	regs[27] = reinterpret_cast<void*>(exceptionContext->x27);
	regs[28] = reinterpret_cast<void*>(exceptionContext->x28);
	regs[29] = reinterpret_cast<void*>(exceptionContext->x29);
	regs[30] = reinterpret_cast<void*>(exceptionContext->x30);

	for (size_t i = 0; i <= 30; i++) {
		if (i < 10) {
			panic << "   x" << i << ": ";
		} else {
			panic << "  x" << i << ": ";
		}

		panic.width(10);
		panic << regs[i];
		panic.width(0);


		if (i % 3 == 2)
			panic << "\n\r";
	}
	panic << "\n\n\r";


	/* Generate stack trace */
	debug::stack_trace(15, reinterpret_cast<void*>(exceptionContext->x29));

	/* Endless loop */
	while (1)
		CPU::halt();
}
