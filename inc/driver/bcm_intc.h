#ifndef _INC_DRIVER_BCM_INTC_H_
#define _INC_DRIVER_BCM_INTC_H_

#include <cstdint.h>
#include <kernel/utility.h>
#include <driver/generic_intc.h>

/**
 * @file driver/bcm_intc.h
 * @brief Driver for Broadcom 2835 Interrupt Controller
 */

namespace driver {

	/**
	 * @class bcm_intc
	 * @brief Broadcom 2835 interrupt controller
	 */
	class bcm_intc : public generic_intc {
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
				irq_basic_pending  = 0xB200,
				irq_pending_1      = 0xB204,
				irq_pending_2      = 0xB208,
				fiq_control        = 0xB20c,
				enable_irqs_1      = 0xB210,
				enable_irqs_2      = 0xB214,
				enable_basic_irqs  = 0xB218,
				disable_irqs_1     = 0xB21c,
				disable_irqs_2     = 0xB220,
				disable_basic_irqs = 0xB224,
			} regOffset;

			/**
			 * @var handlers
			 * @brief Available handlers
			 */
			static lib::function<int()> handlers[96];

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
			 * @fn bcm_intc
			 * @brief Constuctor of driver with sets name
			 */
			bcm_intc();

			/**
			 * @fn int init(const config& conf)
			 * @brief Intialize interrupt controller
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int init(const config& conf);

			/**
			 * @fn int registerHandler(void* data, size_t size, lib::function<int()> handler)
			 * @brief Register handler for driver specific configuration
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int registerHandler(void* data, size_t size, lib::function<int()> handler);

			/**
			 * @fn int handleIRQ()
			 * @brief Handle pending irq
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int handleIRQ();

	};
} /* namespace driver */

#endif /* ifndef _INC_DRIVER_BCM_INTC_H_ */
