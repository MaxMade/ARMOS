static void csu_init() {
	extern void (*__init_array_start []) ();
	extern void (*__init_array_end[]) ();

	long size = __init_array_end - __init_array_start;
	for (long i = 0; i < size; i++) {
		(*__init_array_start[i])();
	}
}

extern "C" void kernelMainWrapper(void *fdt, int cpuID) {
	(void) fdt;
	(void) cpuID;

	if (cpuID == 0) {
		/* Call Global constructors */
		csu_init();

		/* TODO: Call kernelMain */

	} else {
		/* TODO: Call kernelMainApp */
	}
}
