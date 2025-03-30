#include <driver/drivers.h>
#include <kernel/device_tree/parser.h>

driver::console console;

int kernelMain(void *fdt) {
	/* Start parsing device tree */
	DeviceTree::Parser dtp(fdt);
	if (!dtp.isValid())
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
