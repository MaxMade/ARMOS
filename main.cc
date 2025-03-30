#include <kernel/mm/paging.h>
#include <driver/drivers.h>
#include <kernel/cpu.h>
#include <kernel/math.h>
#include <kernel/error.h>
#include <kernel/device_tree/parser.h>
#include <kernel/irq/exception_handler.h>
#include <kernel/mm/translation_table.h>
#include <kernel/mm/translation_table_allocator.h>
#include <hw/register/tcr.h>
#include <hw/register/mair.h>
#include <hw/register/sctlr.h>

driver::console console;

namespace mm {
	TTAllocator ttAlloc;
}

int kernelMain(void *fdt) {
	/* Disable all interrupts */
	CPU::disableInterrupts();

	/* Prepare exeption vector */
	CPU::loadExeptionVector(irq::getExceptionVector());

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
	auto consoleConfig = dtp.findConfig(console);
	if (!consoleConfig.isValid())
		return -1;
	if (!console.init(consoleConfig))
		return -1;
	console.write("Console: Setup finished\n\r", 25);

	return 0;
}
