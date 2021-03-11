#include <new.h>
#include <cerrno.h>
#include <cassert.h>
#include <cstring.h>
#include <kernel/cpu.h>
#include <kernel/error.h>
#include <kernel/lock/softirq.h>
#include <driver/cpu.h>
#include <driver/drivers.h>
#include <driver/generic_driver.h>

using namespace lock;

int Softirq::init() {
	numCPUs = driver::cpus.numCPUs();
	numDrivers = driver::generic_driver::getNumDrivers();

	drivers = new driver::generic_driver*[numCPUs * numDrivers];
	if (drivers == nullptr)
		return -ENOMEM;
	memset(drivers, 0, sizeof(void*) * numCPUs * numDrivers);

	return 0;
}

int Softirq::execute(driver::generic_driver* driver) {
	/* Assert that interrupts are currently disabled */
	assert(CPU::areInterruptsEnabled() == false);

	/* Execute Prologue */
	auto retPrologue = driver->prologue();
	if (isError(retPrologue))
		return retPrologue;

	/* Save current CPU ID */
	auto cpuID = CPU::getProcessorID();
	assert(cpuID < numCPUs);

	/* Save current driver ID */
	auto driverID = driver->getIndexDriver();
	assert(driverID < numDrivers);

	/* Mark softirq layer as used (if possible) */
	auto currentlyUsed = used.get().test_and_set();

	/* If currently an epilogue is executed postpone driver (if necessary) */
	if (currentlyUsed && retPrologue == 1) {
		/* Check if driver is already pending */
		auto alreadyPending = (drivers[cpuID * numDrivers + driverID] != nullptr);
		/* Update driver */
		drivers[cpuID * numDrivers + driverID] = driver;
		/* Update number of pending drivers (if necessary) */
		pendingDrivers.get() += alreadyPending ? 0 : 1;
		return 0;
	}

	/* Execute current epilogue */
	if (retPrologue == 1) {
		CPU::enableInterrupts();
		auto retEpilogue = driver->epilogue();
		CPU::disableInterrupts();
		if (isError(retEpilogue)) {
			/* Mark softirq as unused */
			used.get().clear();
			return retEpilogue;
		}
	}

	/* Execute postponed drivers */
	while (pendingDrivers.get() > 0) {
		for (size_t i = 0; i < numDrivers; i++) {
			driver::generic_driver* postponedDriver = nullptr;

			if (drivers[cpuID * numDrivers + i] != nullptr) {
				lib::swap(postponedDriver, drivers[cpuID * numDrivers + i]);
				pendingDrivers.get()--;

				/* Excute postponed driver */
				CPU::enableInterrupts();
				auto retEpilogue = postponedDriver->epilogue();
				CPU::disableInterrupts();

				/* Check for error condition */
				if (isError(retEpilogue)) {
					/* Mark softirq as unused */
					used.get().clear();
					return retEpilogue;
				}
			}
		}
	}


	/* Mark softirq layer as unused */
	used.get().clear();

	return 0;
}
