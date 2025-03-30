#include <driver/mini_uart.h>

using namespace driver;

mini_uart::mini_uart() {
	name = ("brcm,bcm2835-aux-uart");
}

bool mini_uart::init(const config& conf) {
	/* Set base */
	base = conf.getRange().first;

	/* Enable Mini UART */
	writeRegister<AUX_ENABLES>(1);

	/* Disable transmit/recieve/flow control */
	writeRegister<AUX_MU_CNTL_REG>(0);

	/* Disable transmit/recieve interrupts */
	writeRegister<AUX_MU_IIR_REG>(0);

	/* Enable 8 bit mode */
	writeRegister<AUX_MU_LCR_REG>(3);

	/* Set RTS line to be always high */
	writeRegister<AUX_MU_MCR_REG>(0);

	/* Set baudrate to 115200
	 * TODO: Currently, the system clock is expected to run at 250MHz.
	 */
	writeRegister<AUX_MU_BAUD_REG>(270);

	/* Clear fifo */
	writeRegister<AUX_MU_IER_REG>(0b110);

	/* Enable transmit/recieve */
	writeRegister<AUX_MU_CNTL_REG>(0b11);

	/* Enable recieve interrupt */
	writeRegister<AUX_MU_IIR_REG>(0b01);

	return true;
}

void mini_uart::write(const char* buf, size_t len) {
	for (size_t i = 0; i < len; i++) {
		while ((readRegister<AUX_MU_LSR_REG>() & (1 << 5)) == 0);
		writeRegister<AUX_MU_IO_REG>(buf[i]);
	}
}

char mini_uart::read() {
	while ((readRegister<AUX_MU_LSR_REG>() & 1) == 0);
	return readRegister<AUX_MU_IO_REG>() & 0xFF;
}
