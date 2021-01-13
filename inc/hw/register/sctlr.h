#ifndef _INC_HW_REGISTER_SCTLR_H_
#define _INC_HW_REGISTER_SCTLR_H_

#include <cstdint.h>

/**
 * @file hw/register/sctlr.h
 * @brief System Control Register (EL1)
 * @see
 * D13.2.113 SCTLR_EL1, System Control Register (EL1)
 * Arm Architecture Reference Manual Armv8, for Armv8-A architecture profile
 */

namespace hw::reg {

/**
 * @class SCTLR
 * @brief Abstraction of SCTLR
 */
class SCTLR {
	private:
		typedef union sctlr {
			struct {
				uint64_t m:1;     /**< Bit 00: MMU Enable for EL1 and EL0 stage 1 address translations */
				uint64_t a:1;     /**< Bit 01: Alignment check enable for EL1 and EL0 */
				uint64_t c:1;     /**< Bit 02: Cachebility Stage 1 Control for data accesses */
				uint64_t sa:1;    /**< Bit 03: SP Alignment Check Enable EL1 (16 bytes) */
				uint64_t sa0:1;   /**< Bit 04: SP Alignment Check Enable EL0 (16 bytes) */
				uint64_t res0:4;  /**< Bit 05: Reserved */
				uint64_t uma:1;   /**< Bit 09: User Mask Access (Traps MSR/MRS in EL0) */
				uint64_t res1:2;  /**< Bit 10: Reserved */
				uint64_t i:1;     /**< Bit 12: Cachebiility State 1 Control for instruction accesses */
				uint64_t res2:1;  /**< Bit 13: Reserved */
				uint64_t dze:1;   /**< Bit 14: Trap EL0 execution of DC ZVA instructions */
				uint64_t uct:1;   /**< Bit 15: Trap EL0 access of CTR_EL0 */
				uint64_t ntwi:1;  /**< Bit 16: Trap EL0 execution of WFI instruction */
				uint64_t res3:1;  /**< Bit 17: Reserved */
				uint64_t ntwe:1;  /**< Bit 18: Trap EL0 execution of WFE instruction */
				uint64_t wxn:1;   /**< Bit 19: Write Permission implies XN (Execute-Never) */
				uint64_t res4:4;  /**< Bit 20: Reserved */
				uint64_t e0e:1;   /**< Bit 24: Endianness of data accesses at EL0 (0 = little, 1 = big) */
				uint64_t ee:1;    /**< Bit 25: Endianness of data accesses at EL1 (0 = little, 1 = big) */
				uint64_t uci:1;   /**< Bit 26: Trap EL0 execution of change maintence instructions */
				uint64_t res5:37; /**< Bit 27: Reserved */
			};
			uint64_t value;
		} sctlr_t;

		/**
		 * @var value
		 * @brief Underlying value of SCTLR
		 */
		sctlr_t value;

		/**
		 * @fn void read()
		 * @brief Read current value of SCTLR into value
		 */
		void read();

		/**
		 * @fn void write() const
		 * @brief Write value back into SCTLR
		 */
		void write() const;

	public:
		/**
		 * @fn SCTLR
		 * @brief Read current SCTLR register and construct SCTLR object
		 */
		SCTLR();

		SCTLR(const SCTLR& other) = delete;

		SCTLR(SCTLR&& other) = delete;

		SCTLR& operator=(const SCTLR& other) = delete;

		SCTLR& operator=(SCTLR&& other) = delete;

		/**
		 * @fn bool isMMUEnabled() const
		 * @brief Check if MMU is enabled
		 */
		bool isMMUEnabled() const;

		/**
		 * @fn bool isAlignmentCheckEnabled() const
		 * @brief Check if alignment check is enabled
		 */
		bool isAlignmentCheckEnabled() const;

		/**
		 * @fn bool isDataCachable() const
		 * @brief Check if caching of data accesses is enabled
		 */
		bool isDataCachable() const;

		/**
		 * @fn bool isSP1AlignCheckEnabled() const
		 * @brief Check if SP EL1 alignment checks are enabled
		 */
		bool isSP1AlignCheckEnabled() const;

		/**
		 * @fn bool isSP0AlignCheckEnabled() const
		 * @brief Check if SP EL0 alignment checks are enabled
		 */
		bool isSP0AlignCheckEnabled() const;

		/**
		 * @fn bool isUserMaskAccessEnabled() const
		 * @brief Check if user mask accesses are enabled
		 */
		bool isUserMaskAccessEnabled() const;

		/**
		 * @fn bool isInstrCachable() const
		 * @brief Check if caching of instruction acccesses is enabled
		 */
		bool isInstrCachable() const;

		/**
		 * @fn bool writePermissionImplNX() const
		 * @brief Check if write permission (MMU) implies NX (no execute)
		 */
		bool writePermissionImplNX() const;

		/**
		 * @fn bool isEL0LittleEndian() const
		 * @brief Check if data accessess of EL0 expect little endianness
		 */
		bool isEL0LittleEndian() const;

		/**
		 * @fn bool isEL1LittleEndian() const
		 * @brief Check if data accessess of EL1 expect little endianness
		 */
		bool isEL1LittleEndian() const;

		/**
		 * @fn void setMMUEnabled(bool enabled)
		 * @brief Enable or disable MMU
		 */
		void setMMUEnabled(bool enabled);

		/**
		 * @fn void setAlignmentCheckEnabled(bool enabled)
		 * @brief Enable or disable alignment check
		 */
		void setAlignmentCheckEnabled(bool enabled);

		/**
		 * @fn void setDataCachable(bool enabled)
		 * @brief Enable or disablecaching of data accesses
		 */
		void setDataCachable(bool enabled);

		/**
		 * @fn void setSP1AlignCheckEnabled(bool enabled)
		 * @brief Enable or disable SP EL1 alignment checks
		 */
		void setSP1AlignCheckEnabled(bool enabled);

		/**
		 * @fn void setSP0AlignCheckEnabled(bool enabled)
		 * @brief Enable or disable SP EL0 alignment checks
		 */
		void setSP0AlignCheckEnabled(bool enabled);

		/**
		 * @fn void setUserMaskAccessEnabled(bool enabled)
		 * @brief Enable or disable user mask accesses
		 */
		void setUserMaskAccessEnabled(bool enabled);

		/**
		 * @fn void setInstrCachable(bool enabled)
		 * @brief Enable or disable caching of instruction acccesses
		 */
		void setInstrCachable(bool enabled);

		/**
		 * @fn void setWritePermsetsionImplNX(bool enabled)
		 * @brief Enable or disable write permission (MMU) implies NX (no execute)
		 */
		void setWritePermsetsionImplNX(bool enabled);

		/**
		 * @fn void setEL0LittleEndian(bool enabled)
		 * @brief Set data accessess of EL0 little endianness
		 */
		void setEL0LittleEndian(bool enabled);

		/**
		 * @fn void setEL1LittleEndian(bool enabled)
		 * @brief Check if data accessess of EL1 little endianness
		 */
		void setEL1LittleEndian(bool enabled);
};

} /* namespace hw::reg */

#endif /* ifndef _INC_HW_REGISTER_SCTLR_H_ */
