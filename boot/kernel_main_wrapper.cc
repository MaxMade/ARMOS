extern "C" void kernelMainWrapper(void *fdt, int cpuID) {
	(void) fdt;
	(void) cpuID;

	if (cpuID == 0) {
		/* TODO: Call Global constructors */
		/* TODO: Call kernelMain */

	} else {
		/* TODO: Call kernelMainApp */
	}
}
