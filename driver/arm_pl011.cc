#include <driver/arm_pl011.h>

using namespace driver;

arm_pl011::arm_pl011() {
	name = ("arm,pl011");
}

bool arm_pl011::init(const config& conf) {
	/* Set base */
	base = conf.getRange().first;

	/* Disable console */
	writeRegister<CONTROL_REG>(0);

	/* Clear interrupt pending flag */
	writeRegister<INTERRUPT_CLEAR_REG>(0x7FF);

	/* TODO Set baudrate to 115200
	 * Fixed values for fixed UART clock rate of 4MHz
	 */
	writeRegister<INTEGER_BAUD_RATE_REG>(0x1);
	writeRegister<FRACTIONAL_BAUD_RATE_REG>(0x28);

	/* Default setting:
	 * - Data bits: 8
	 * - Stop bits: 1
	 * - Parity: None
	 */
	writeRegister<LINE_CONTROL_REG>((1 << 4) | (1 << 5) | (1 << 6));

	/* Enable RX interrupts */
	writeRegister<INTERRUPT_MASK_REG>((1 << 1) | (1 << 4) | (1 << 5) | (1 << 6)
			| (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

	/* Enable console */
	writeRegister<CONTROL_REG>((1 << 0) | (1 << 8) | (1 << 9));

	return true;
}

void arm_pl011::write(const char* buf, size_t len) {
	for (size_t i = 0; i < len; i++) {
		while ((readRegister<FLAG_REG>() & (1 << 5)) != 0);
		writeRegister<DATA_REG>(buf[i]);
	}
}

char arm_pl011::read() {
	while ((readRegister<FLAG_REG>() & (1 << 4)) != 0);
	return readRegister<DATA_REG>();
}


lib::pair<void*, size_t> arm_pl011::getConfigSpace() const {
	return lib::pair(base, 0x200);
}
