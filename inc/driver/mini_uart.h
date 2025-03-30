#ifndef _INC_DRIVER_MINI_UART_H_
#define _INC_DRIVER_MINI_UART_H_

#include <cstdint.h>
#include <kernel/utility.h>
#include <driver/config.h>
#include <driver/generic_console.h>

/**
 * @file driver/mini_uart.h
 * @brief Driver for BCM2835 Mini UART
 */

namespace driver {

	class mini_uart : public generic_console {
		private:
			/**
			 * @var base
			 * @brief Base address of configuration space
			 */
			void* base;

			lib::pair<void*, size_t> intConfig;

			/**
			 * @enum regOffset
			 * @brief Offsets for registers
			 */
			typedef enum : int16_t {
				AUX_ENABLES     = -0x3c,
				AUX_MU_IO_REG   =  0x00,
				AUX_MU_IIR_REG  =  0x04,
				AUX_MU_IER_REG  =  0x08,
				AUX_MU_LCR_REG  =  0x0C,
				AUX_MU_MCR_REG  =  0x10,
				AUX_MU_LSR_REG  =  0x14,
				AUX_MU_MSR_REG  =  0x18,
				AUX_MU_SCRATCH  =  0x1C,
				AUX_MU_CNTL_REG =  0x20,
				AUX_MU_STAT_REG =  0x24,
				AUX_MU_BAUD_REG =  0x28,
			} regOffset;

			/**
			 * @fn void writeRegister(uint32_t value)
			 * @brief Internal write register
			 */
			template<regOffset off>
			void writeRegister(uint32_t value) {
				uint32_t* reg = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(base) + off);
				util::mmioWrite(reg, value);
			}

			/**
			 * @fn void readRegister(uint32_t value)
			 * @brief Internal read register
			 */
			template<regOffset off>
			uint32_t readRegister() {
				uint32_t* reg = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(base) + off);
				return util::mmioRead(reg);
			}

		public:
			/**
			 * @fn mini_uart
			 * @brief Constuctor of driver with sets name
			 */
			mini_uart();

			/**
			 * @fn bool init(const config& conf)
			 * @brief Intialize console
			 * @todo Current the baudrate is calculated for a fixed clock rate of 4MHz
			 */
			bool init(const config& conf);

			/**
			 * @fn void write(const char* buf, size_t len)
			 * @brief Write buffer to console
			 */
			void write(const char* buf, size_t len);

			/**
			 * @fn char read()
			 * @brief Read a single byte
			 */
			char read();
	};

} /* namespace driver */

#endif /* ifndef _INC_DRIVER_MINI_UART_H_ */
