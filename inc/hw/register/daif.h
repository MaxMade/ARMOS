#ifndef _INC_HW_REGISTER_DAIF_H_
#define _INC_HW_REGISTER_DAIF_H_

#include <cstdint.h>

/**
 * @file hw/register/daif.h
 * @brief Debug/Asynchronious/IRQ/FIQ mask of PSTATE
 * @see
 * C5.2.2 DAIF, Interrupt Mask Bits
 * Arm Architecture Reference Manual Armv8, for Armv8-A architecture profile
 */

namespace hw::reg {

/**
 * @class DAIF
 * @brief Abstraction of DAIF
 */
class DAIF {
	private:
		typedef union daif {
			struct {
				uint64_t res0:6;
				uint64_t fiq:1;
				uint64_t irq:1;
				uint64_t async:1;
				uint64_t debug:1;
				uint64_t res1:54;
			};
			uint64_t value;
		} daif_t;

		/**
		 * @var value
		 * @brief Underlying value of DAIF
		 */
		daif_t value;

		/**
		 * @fn void read()
		 * @brief Read current value of DAIF into value
		 */
		void read();

		/**
		 * @fn void write() const
		 * @brief Write value back into DAIF
		 */
		void write() const;

	public:
		/**
		 * @fn DAIF
		 * @brief Read current DAIF register and construct DAIF object
		 */
		DAIF();

		DAIF(const DAIF& other) = delete;

		DAIF(DAIF&& other) = delete;

		DAIF& operator=(const DAIF& other) = delete;

		DAIF& operator=(DAIF&& other) = delete;

		/**
		 * @fn bool isDebugMasked() const
		 * @brief Check if debug interrupts are masked
		 */
		bool isDebugMasked() const;

		/**
		 * @fn bool isDebugMasked() const
		 * @brief Check if SError interrupts are masked
		 */
		bool isAsyncMasked() const;

		/**
		 * @fn bool isIrqMasked() const
		 * @brief Check if IRQs are masked
		 */
		bool isIrqMasked() const;

		/**
		 * @fn bool isFiqMasked() const
		 * @brief Check if FIQs are masked
		 */
		bool isFiqMasked() const;

		/**
		 * @fn void setDebugMasked(bool masked)
		 * @brief Set mask for debug interrupts
		 */
		void setDebugMasked(bool masked);

		/**
		 * @fn void setAsyncMasked(bool masked)
		 * @brief Set mask for SError interrupts
		 */
		void setAsyncMasked(bool masked);

		/**
		 * @fn void setIrqMasked(bool masked)
		 * @brief Set mask for IRQs
		 */
		void setIrqMasked(bool masked);

		/**
		 * @fn void setFiqMasked(bool masked)
		 * @brief Set mask for FIQs
		 */
		void setFiqMasked(bool masked);
};

} /* namespace hw::reg */

#endif /* ifndef _INC_HW_REGISTER_DAIF_H_ */
