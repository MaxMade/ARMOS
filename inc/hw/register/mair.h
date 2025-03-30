#ifndef _INC_HW_REGISTER_MAIR_H_
#define _INC_HW_REGISTER_MAIR_H_

/**
 * @file hw/register/mair.h
 * @brief Memory Attribute Indirection Register (EL1)
 * @details
 * @see
 * D13.2.92 MAIR_EL1, Memory Attribute Indirection Register (EL1)
 * Arm Architecture Reference Manual Armv8, for Armv8-A architecture profile
 */

#include <cstdint.h>
#include <cstddef.h>

namespace hw::reg {

/**
 * @class MAIR
 * @brief Abstraction of MAIR
 */
class MAIR {
	private:
		/**
		 * @var NUM_ENTRIES
		 * @brief Number of entries in MAIR
		 */
		static const size_t NUM_ENTRIES = 8;

		typedef union mair {
			uint8_t attrs[NUM_ENTRIES];
			uint64_t value;
		} mair_t;

		/**
		 * @var value
		 * @brief Underlying value of MAIR
		 */
		mair_t value;

		/**
		 * @fn void read()
		 * @brief Read current value of MAIR into value
		 */
		void read();

		/**
		 * @fn void write() const
		 * @brief Write value back into MAIR
		 */
		void write() const;

	public:
		/**
		 * @fn MAIR
		 * @brief Read current MAIR register and construct MAIR object
		 */
		MAIR();

		MAIR(const MAIR& other) = delete;

		MAIR(MAIR&& other) = delete;

		MAIR& operator=(const MAIR& other) = delete;

		MAIR& operator=(MAIR&& other) = delete;

		/**
		 * @typedef dev_attr_t
		 * @brief Device memory attributes
		 */
		typedef enum {
			ngnrne = 0b00, /**< Device, non-gathering, non-reodering, no early write acknowledgement */
			ngnre  = 0b01, /**< Device, non-gathering, non-reodering, early write acknowledgement */
			ngre   = 0b10, /**< Device, non-gathering, redering, early write acknowledgement */
			gre    = 0b11, /**< Device, gathering, reodering, early write acknownledgement */
		} dev_attr_t;


		/**
		 * @typedef normal_attr_t
		 * @brief Normal memory attributes
		 */
		typedef enum {
			wttnraw = 0b0001, /**< Normal, write-through transient, no-allocate read, allocate write */
			wttarnw = 0b0010, /**< Normal, write-through transient, allocate read, no-allocate write */
			wttaraw = 0b0011, /**< Normal, write-through transient, allocate read, allocate write */

			nc      = 0b0100, /**< Normal memory, non-cacheable */

			wbtnraw = 0b0101, /**< Normal, write-back transient, no-allocate read, allocate write */
			wbtarnw = 0b0110, /**< Normal, write-back transient, allocate read, no-allocate write */
			wbtaraw = 0b0111, /**< Normal, write-back transient, allocate read, allocate write */

			wtntnraw = 0b1001, /**< Normal, write-through non-transient, no-allocate read, allocate write */
			wtntarnw = 0b1010, /**< Normal, write-through non-transient, allocate read, no-allocate write */
			wtntaraw = 0b1011, /**< Normal, write-through non-transient, allocate read, allocate write */

			wbntnraw = 0b1101, /**< Normal, write-back non-transient, no-allocate read, allocate write */
			wbntarnw = 0b1110, /**< Normal, write-back non-transient, allocate read, no-allocate write */
			wbntaraw = 0b1111, /**< Normal, write-back non-transient, allocate read, allocate write */
		} normal_attr_t;

		/**
		 * @fn int getAttribute(size_t entry)
		 * @brief Get attribute for entry
		 * @warning entry should be less than 8
		 * @return
		 *
		 *	-  0 - Success
		 *	- <0 - Failure (-errno)
		 */
		int getAttribute(size_t entry);

		/**
		 * @fn int setDeviceAttribute(size_t entry, dev_attr_t attr)
		 * @brief Set device attribute for entry
		 * @warning entry should be less than 8
		 * @return
		 *
		 *	-  0 - Success
		 *	- <0 - Failure (-errno)
		 */
		int setDeviceAttribute(size_t entry, dev_attr_t attr);

		/**
		 * @fn int setNormalAttribute(size_t entry, normal_attr_t inner, normal_attr_t outer)
		 * @brief Set normal attribute for entry
		 * @warning entry should be less than 8
		 * @return
		 *
		 *	-  0 - Success
		 *	- <0 - Failure (-errno)
		 */
		int setNormalAttribute(size_t entry, normal_attr_t inner, normal_attr_t outer);

		/**
		 * @fn void useDefaultLayout()
		 * @brief Set MAIR to default layout
		 * @details:
		 * The first entry of the MAIR will contain the device memory setting
		 * (ngnrne) while the second entry will be used for normal memory
		 * (outer non-cacheable, inner non-cacheable). All other entries will
		 * remain the same as before.
		 */
		void useDefaultLayout();
};

} /* namespace hw::reg */

#endif /* ifndef _INC_HW_REGISTER_MAIR_H_ */
