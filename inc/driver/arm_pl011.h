#ifndef _INC_DRIVER_ARM_PL011_H_
#define _INC_DRIVER_ARM_PL011_H_

#include <cstdint.h>
#include <kernel/utility.h>
#include <driver/config.h>
#include <driver/generic_console.h>

/**
 * @file driver/arm_pl011.h
 * @brief Driver for ARM PL011
 */

namespace driver {

	class arm_pl011 : public generic_console {
		private:
			/**
			 * @var base
			 * @brief Base address of configuration space
			 */
			void* base;

			/**
			 * @enum regOffset
			 * @brief Offsets for registers
			 */
			typedef enum : uint16_t {
				DATA_REG                 = 0x00,
				FLAG_REG                 = 0x18,
				INTEGER_BAUD_RATE_REG    = 0x24,
				FRACTIONAL_BAUD_RATE_REG = 0x28,
				LINE_CONTROL_REG         = 0x2C,
				CONTROL_REG              = 0x30,
				INTERRUPT_MASK_REG       = 0x38,
				INTERRUPT_CLEAR_REG      = 0x44,
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
			 * @fn arm_pl011
			 * @brief Constuctor of driver with sets name
			 */
			arm_pl011();

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

#endif /* ifndef _INC_DRIVER_ARM_PL011_H_ */
