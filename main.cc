#include <cassert.h>
#include <ios.h>
#include <ostream.h>
#include <driver/cpu.h>
#include <driver/drivers.h>
#include <kernel/cpu.h>
#include <kernel/math.h>
#include <kernel/error.h>
#include <kernel/linker.h>
#include <kernel/symbols.h>
#include <kernel/thread/smp.h>
#include <kernel/debug/panic.h>
#include <kernel/device_tree/parser.h>
#include <kernel/irq/exception_handler.h>
#include <kernel/thread/idle.h>
#include <kernel/thread/context.h>
#include <kernel/mm/paging.h>
#include <kernel/mm/translation_table.h>
#include <kernel/mm/translation_table_allocator.h>
#include <kernel/lock/softirq.h>
#include <hw/register/tcr.h>
#include <hw/register/mair.h>
#include <hw/register/sctlr.h>

/* Global objects */
namespace driver {
	Console console;
	Intc intc;
	Timer timer;
	CPUs cpus;
	IPI ipi;
}

namespace mm {
	TTAllocator ttAlloc;
}

namespace thread {
	SMP smp;
	IdleThreads idleThreads;
}

namespace lock {
	Softirq softirq;
}

Symbols symbols;

thread::Context mainThread;

extern "C" int main();

/* User stack for main application */
static char userStack[STACK_SIZE] __attribute__((aligned(PAGESIZE),section("app.bss")));

int kernelMain(void *fdt) {
	/* Disable all interrupts */
	CPU::disableInterrupts();

	/* Prepare exeption vector */
	CPU::loadExeptionVector(irq::getExceptionVector());

	/* Prepare symbol map */
	if (!symbols.init(linker::getSymbolMapStart()))
		return -1;

	/* Start parsing device tree */
	DeviceTree::Parser dtp(fdt);
	if (!dtp.isValid())
		return -1;

	/* Initialize Translation Table Allocator */
	mm::ttAlloc.init();

	/* Use default MAIR layout */
	hw::reg::MAIR mair;
	mair.useDefaultLayout();

	/* Use default TCR layout */
	hw::reg::TCR tcr;
	tcr.useDefaultSetting();

	/* Create kernel mapping */
	if (isError(mm::Paging::createEarlyKernelMapping()))
		return -1;

	/* Enable MMU */
	hw::reg::SCTLR sctrl;
	sctrl.setMMUEnabled(true);

	/* Map all devices */
	if (isError(dtp.createMapping()))
		return -1;

	/* Prepare interrupt controller */
	auto intcConfig = dtp.findConfig(driver::intc);
	if (!intcConfig.isValid())
		return -1;
	if (isError(driver::intc.init(intcConfig)))
		return -1;

	/* Prepare console */
	auto consoleConfig = dtp.findConfig(driver::console);
	if (!consoleConfig.isValid())
		return -1;
	if (isError(driver::console.init(consoleConfig)))
		return -1;
	driver::console.write("Console: Setup finished\n\r", 25);
	lib::ostream cout;

	/* Prepare timer */
	auto timerConfig = dtp.findConfig(driver::timer);
	if (!timerConfig.isValid()) {
		cout << "Timer: Construction failed!" << lib::endl;
		return -1;
	}

	if (isError(driver::timer.init(timerConfig))) {
		cout << "Timer: Initialization failed!" << lib::endl;
		return -1;
	}
	driver::timer.windup(200);
	cout << "Timer: Setup finished" << lib::endl;

	/* Prepare CPU information */
	for (auto node : dtp) {
		if (!node.isValid())
			continue;

		if (strncmp("cpu@", node.getName(), 4) == 0) {
			const char* cpuName = nullptr;
			void* spintable = nullptr;

			auto comp = node.findStringlistProperty("compatible");
			if (comp.first != nullptr)
				cpuName = comp.first;

			auto relAddr = node.findLongProperty("cpu-release-addr");
			if (comp.first != nullptr)
				spintable = reinterpret_cast<void*>(relAddr.first);

			if (cpuName && spintable) {
				driver::cpus.registerCPU(driver::CPU(cpuName, spintable));
			}
		}
	}
	cout << "CPUs: Setup finished" << lib::endl;

	/* Prepare IPI driver */
	auto ipiConfig = dtp.findConfig(driver::ipi);
	if (!ipiConfig.isValid())
		return -1;
	if (isError(driver::ipi.init(ipiConfig)))
		return -1;
	cout << "IPI: Setup finished" << lib::endl;

	/* Prepare panic */
	if (debug::panic::init() != 0) {
		cout << "PANIC: Unable to initialize" << lib::endl;
		return -1;
	}
	cout << "PANIC: Setup finished" << lib::endl;

	/* Prepare softirq */
	if (isError(lock::softirq.init()))
		debug::panic::generate("Softirq: Unable to initialize");
	cout << "Softirq: Setup finished" << lib::endl;

	/* Prepare Idle Threads */
	if (thread::idleThreads.init() != 0)
		debug::panic::generate("Thread: Unable to initialize idle thread");
	cout << "Thread: Idle Thread Setup finished" << lib::endl;

	/* Prepare SMP */
	if (thread::smp.start() != 0)
		debug::panic::generate("SMP: Unable to initialize");
	cout << "SMP: Setup finished" << lib::endl;

	/* Unmap page 0x0 */
	mm::Paging paging;
	paging.unmap(nullptr);
	CPU::invalidatePage(nullptr);

	cout << "CPU " << CPU::getProcessorID() << ": Finished initialization" << lib::endl;

	/* Prepare Main Thread */
	char* kernelStack = new char[STACK_SIZE];
	if (kernelStack == nullptr)
		debug::panic::generate("Thread: Unable to allocate kernel stack for main thread");
	mainThread.init(0, kernelStack, userStack, false, (void*) main);
	cout << "Thread: Setup of main thread finished" << lib::endl;

	/* Preform initial context switch */
	thread::Context tmpContext;
	thread::Context::switching(&tmpContext, &mainThread);

	debug::panic::generate("End of kernelMain must never be reached!");
	return -1;
}

int kernelMainApp() {
	/* Disable all interrupts */
	CPU::disableInterrupts();

	/* Prepare exeption vector */
	CPU::loadExeptionVector(irq::getExceptionVector());

	/* Register CPU */
	thread::smp.registerCPU();

	/* Use default MAIR layout */
	hw::reg::MAIR mair;
	mair.useDefaultLayout();

	/* Use default TCR layout */
	hw::reg::TCR tcr;
	tcr.useDefaultSetting();

	/* Load kernel mapping */
	mm::Paging::loadKernelMapping();

	/* Enable MMU */
	hw::reg::SCTLR sctrl;
	sctrl.setMMUEnabled(true);

	/* Local output stream */
	lib::ostream cout;

	/* Prepare Idle Thread */
	if (thread::idleThreads.init() != 0)
		debug::panic::generate("Thread: Unable to initialize idle thread");
	cout << "Thread: Idle Thread Setup finished" << lib::endl;

	cout << "CPU " << CPU::getProcessorID() << ": Finished initialization" << lib::endl;

	/* Preform initial context switch */
	thread::Context tmpContext;
	auto& idleThread = thread::idleThreads.get();
	thread::Context::switching(&tmpContext, &idleThread);

	debug::panic::generate("End of kernelMain must never be reached!");
	return -1;
}
