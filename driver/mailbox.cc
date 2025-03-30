#include <cerrno.h>
#include <kernel/cpu.h>
#include <driver/drivers.h>
#include <driver/mailbox.h>

using namespace driver;

mailbox::mailbox() {
	name = ("brcm,bcm2835-mbox");
}

int mailbox::init(const config& conf) {
	/* Set base */
	base = conf.getRange().first;

	/* Prepare interrupt configuration */
	intConfig.first = conf.getInterruptRange().first;
	intConfig.second = conf.getInterruptRange().second;

	/* Register interrupt controller */
	if (int err = intc.registerHandler(intConfig.first, intConfig.second, this); err)
		return err;

	/* Enable IRQs */
	enableIRQ(0);
	enableIRQ(1);
	enableIRQ(2);
	enableIRQ(3);

	return 0;
}


int mailbox::sendIPI(size_t cpuID, IPI_MSG msg) {
	if (cpuID == CPU::getProcessorID())
		return -EINVAL;

	if (static_cast<int>(msg) == 0)
		return -EINVAL;

	lock.lock();

	if (cpuID == 0) {
		writeRegister<MAILBOX_WRITE_CORE_0>(static_cast<uint32_t>(msg));

	} else if (cpuID == 1) {
		writeRegister<MAILBOX_WRITE_CORE_1>(static_cast<uint32_t>(msg));

	} else if (cpuID == 2) {
		writeRegister<MAILBOX_WRITE_CORE_2>(static_cast<uint32_t>(msg));

	} else if (cpuID == 3) {
		writeRegister<MAILBOX_WRITE_CORE_3>(static_cast<uint32_t>(msg));
	}

	while (messages[cpuID].load() == 0);

	lock.unlock();

	return 0;
}

int mailbox::registerHandler(IPI_MSG msg, lib::function<int()> handler) {
	for (size_t i = 0; i < numHandlers; i++) {
		/* Find valid entry */
		if (!handlers[i].second.isValid()) {
			/* Register handler */
			handlers[i] = lib::pair(msg, lib::move(handler));
			if (handlers[i].second.isValid())
				return 0;

			/* Abort if registration failed */
			return -ENOMEM;
		}
	}
	return -ENOMEM;
}

void mailbox::enableIRQ(size_t core) {
	if (core == 0) {
		writeRegister<MAILBOX_IRQ_CORE_0>(1);

	} else if (core == 1) {
		writeRegister<MAILBOX_IRQ_CORE_1>(1);

	} else if (core == 2) {
		writeRegister<MAILBOX_IRQ_CORE_2>(1);

	} else if (core == 3) {
		writeRegister<MAILBOX_IRQ_CORE_3>(1);
	}
}

int mailbox::prologue(irq::ExceptionContext* context) {
	(void) context;

	auto cpuID = CPU::getProcessorID();
	if (cpuID >= 4)
		return -EINVAL;

	/* Read message and acknowledge interrupt */
	uint32_t msg = 0;
	if (cpuID == 0) {
		msg = readRegister<MAILBOX_READ_CORE_0>();
		writeRegister<MAILBOX_READ_CORE_0>(msg);

	} else if (cpuID == 1) {
		msg = readRegister<MAILBOX_READ_CORE_1>();
		writeRegister<MAILBOX_READ_CORE_1>(msg);

	} else if (cpuID == 2) {
		msg = readRegister<MAILBOX_READ_CORE_2>();
		writeRegister<MAILBOX_READ_CORE_2>(msg);

	} else if (cpuID == 3) {
		msg = readRegister<MAILBOX_READ_CORE_3>();
		writeRegister<MAILBOX_READ_CORE_3>(msg);
	}

	/* Buffer message for epilogue */
	messages[cpuID].fetch_or(msg);

	return 1;
}

int mailbox::epilogue() {
	auto cpuID = CPU::getProcessorID();
	if (cpuID >= 4)
		return -EINVAL;

	while (messages[cpuID].load()) {
		int performedHandlers = 0;

		for (size_t i = 0; i < numHandlers; i++) {
			/* Check if valid handler */
			auto isValid = handlers[i].second.isValid();
			if (!isValid)
				continue;

			/* Check if handler is pending otherwise continue */
			auto handleredMsg = static_cast<uint32_t>(handlers[i].first);
			if ((messages[cpuID].fetch_and(~handleredMsg) & handleredMsg) == 0)
				continue;

			/* Mark progress */
			performedHandlers++;

			/* Execute handler */
			auto ret = handlers[i].second();
			if (ret != 0)
				return ret;
		}

		/* Check if a suitable handler was found */
		if (performedHandlers == 0)
			return -EINVAL;
	}

	return 0;
}
