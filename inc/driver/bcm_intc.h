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
				irq_basic_pending  = 0x00,
				irq_pending_1      = 0x04,
				irq_pending_2      = 0x08,
				fiq_control        = 0x0c,
				enable_irqs_1      = 0x10,
				enable_irqs_2      = 0x14,
				enable_basic_irqs  = 0x18,
				disable_irqs_1     = 0x1c,
				disable_irqs_2     = 0x20,
				disable_basic_irqs = 0x24,
			} regOffset;

			/**
			 * @var handlers
			 * @brief Available handlers
			 */
			static generic_driver* handlers[96];

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
			 * @fn void readRegister(uint32_t value) const
			 * @brief Internal read register
			 */
			template<regOffset off>
			uint32_t readRegister() const {
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
			 * @fn int registerHandler(void* data, size_t size, generic_driver* driver)
			 * @brief Register driver for driver specific configuration
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int registerHandler(void* data, size_t size, generic_driver* driver);

			/**
			 * @fn generic_driver* getHandler()
			 * @brief Get handler for pending IRQ
			 * @return
			 *
			 *	- Pointer to handler      - Success
			 *	- makeError<generic_driver*>(errno) - Failure
			 */
			generic_driver* getHandler();

			/**
			 * @fn int prologue() override
			 * @brief Exception prologue
			 * @return
			 *
			 *	-  1 - Epilogue is needed
			 *	-  0 - Epilogue isn't needed
			 *	- <0 - Error (errno)
			 */
			int prologue() override;

			/**
			 * @fn int epilogue() override
			 * @brief Exception epilogue
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Error (errno)
			 */
			int epilogue() override;

	};
} /* namespace driver */

#endif /* ifndef _INC_DRIVER_BCM_INTC_H_ */
