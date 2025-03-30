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

	/* Prepare handler */
	auto handler = [this] () mutable {
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

		for (size_t i = 0; i < numHandlers; i++) {
			if (!handlers[i].second.isValid())
				continue;

			if (handlers[i].first != msg)
				continue;

			auto ret = handlers[i].second();
			if (ret != 0)
				return ret;
		}
		return 0;
	};

	/* Register interrupt controller */
	if (int err = intc.registerHandler(intConfig.first, intConfig.second, lib::function<int()>(handler)); err)
		return err;

	/* Enable IRQs */
	enableIRQ(0);
	enableIRQ(1);
	enableIRQ(2);
	enableIRQ(3);

	return 0;
}


int mailbox::sendIPI(size_t cpuID, size_t msg) {
	(void) cpuID;
	(void) msg;

	if (cpuID == 0) {
		writeRegister<MAILBOX_WRITE_CORE_0>(msg);
		return 0;
	}

	if (cpuID == 1) {
		writeRegister<MAILBOX_WRITE_CORE_1>(msg);
		return 0;
	}

	if (cpuID == 2) {
		writeRegister<MAILBOX_WRITE_CORE_2>(msg);
		return 0;
	}

	if (cpuID == 3) {
		writeRegister<MAILBOX_WRITE_CORE_3>(msg);
		return 0;
	}

	return -EINVAL;
}

int mailbox::registerHandler(size_t msg, lib::function<int()> handler) {
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

lib::pair<void*, size_t> mailbox::getConfigSpace() const {
	return lib::pair(base, 0x60);
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
