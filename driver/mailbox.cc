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
	(void) cpuID;
	(void) msg;

	if (cpuID == 0) {
		writeRegister<MAILBOX_WRITE_CORE_0>(static_cast<uint32_t>(msg));
		return 0;
	}

	if (cpuID == 1) {
		writeRegister<MAILBOX_WRITE_CORE_1>(static_cast<uint32_t>(msg));
		return 0;
	}

	if (cpuID == 2) {
		writeRegister<MAILBOX_WRITE_CORE_2>(static_cast<uint32_t>(msg));
		return 0;
	}

	if (cpuID == 3) {
		writeRegister<MAILBOX_WRITE_CORE_3>(static_cast<uint32_t>(msg));
		return 0;
	}

	return -EINVAL;
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
	if (core == 0)
		writeRegister<MAILBOX_IRQ_CORE_0>(1);

	if (core == 1)
		writeRegister<MAILBOX_IRQ_CORE_1>(1);

	if (core == 2)
		writeRegister<MAILBOX_IRQ_CORE_2>(1);

	if (core == 3)
		writeRegister<MAILBOX_IRQ_CORE_3>(1);
}

int mailbox::prologue() {
	auto cpuID = CPU::getProcessorID();
	if (cpuID >= 4)
		return -EINVAL;

	/* Read message and aknowledge interrupt */
	uint32_t msg = 0;
	if (cpuID == 0) {
		msg = readRegister<MAILBOX_READ_CORE_0>();
		writeRegister<MAILBOX_READ_CORE_0>(msg);
	}

	if (cpuID == 1) {
		msg = readRegister<MAILBOX_READ_CORE_1>();
		writeRegister<MAILBOX_READ_CORE_1>(msg);
	}

	if (cpuID == 2) {
		msg = readRegister<MAILBOX_READ_CORE_2>();
		writeRegister<MAILBOX_READ_CORE_2>(msg);
	}

	if (cpuID == 3) {
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

	uint32_t savedMsg = 0;
	savedMsg = messages[cpuID].exchange(savedMsg);
	for (size_t i = 0; i < numHandlers; i++) {
		if (!handlers[i].second.isValid())
			continue;

		if ((static_cast<uint32_t>(handlers[i].first) & savedMsg) == 0)
			continue;

		auto ret = handlers[i].second();
		if (ret != 0)
			return ret;
	}

	return 0;
}
