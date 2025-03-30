#ifndef _INC_HW_REGISTER_TCR_H_
#define _INC_HW_REGISTER_TCR_H_

#include <cstdint.h>
#include <cstddef.h>

/**
 * @file hw/register/tcr.h
 * @brief Translation Control Register (EL1)
 * @see
 * D13.2.120 TCR_EL1, Translation Control Register (EL1)
 * Arm Architecture Reference Manual Armv8, for Armv8-A architecture profile
 */

namespace hw::reg {

/**
 * @class TCR
 * @brief Abstraction of TCR
 */
class TCR {
	private:
		typedef union tcr {
			struct {
				uint64_t t0sz:6;  /**< Bit 00: Region size for TTBR0 (1 << (64 - t0sz)) */
				uint64_t res0:1;  /**< Bit 06: Reserved */
				uint64_t epd0:1;  /**< Bit 07: Translation table walk disable for translations using TTBR0 */
				uint64_t irgn0:2; /**< Bit 08: Inner cacheability attribute for memory associated with translation table walks using TTBR0 */
				uint64_t orgn0:2; /**< Bit 10: Outer cacheability attribute for memory associated with translation table walks using TTBR0 */
				uint64_t sh0:2;   /**< Bit 12: Shareability attribute for memory associated with translation table walks using TTBR0 */
				uint64_t tg0:2;   /**< Bit 14: Granule size for the TTBR0 */
				uint64_t t1sz:6;  /**< Bit 16: Region size for TTBR1 (1 << (64 - t1sz)) */
				uint64_t A1:1;    /**< Bit 22: Select if TTBR0 or TTBR1 defines ASID */
				uint64_t epd1:1;  /**< Bit 23: Translation table walk disable for translations using TTBR0 */
				uint64_t irgn1:2; /**< Bit 24: Inner cacheability attribute for memory associated with translation table walks using TTBR1 */
				uint64_t orgn1:2; /**< Bit 26: Outer cacheability attribute for memory associated with translation table walks using TTBR1 */
				uint64_t sh1:2;   /**< Bit 28: Shareability attribute for memory associated with translation table walks using TTBR1 */
				uint64_t tg1:2;   /**< Bit 30: Granule size for the TTBR1 */
				uint64_t ips:3;   /**< Bit 32: Intermediate Physical Address Size */
				uint64_t res1:1;  /**< Bit 35: Reserved */
				uint64_t as:1;    /**< Bit 36: ASID Size */
				uint64_t tbi0:1;  /**< Bit 37: Top byte ignore using TTBR0 */
				uint64_t tbi1:1;  /**< Bit 38: Top byte ignore using TTBR1 */
				uint64_t res2:25; /**< Bit 39: Reserved */
			};
			uint64_t value;
		} tcr_t;

		/**
		 * @var value
		 * @brief Underlying value of TCR
		 */
		tcr_t value;

		/**
		 * @fn void read()
		 * @brief Read current value of TCR into value
		 */
		void read();

		/**
		 * @fn void write() const
		 * @brief Write value back into TCR
		 */
		void write() const;

	public:

		/**
		 * @enum cachebility
		 * @brief Cacheability Settings
		 */
		enum cachebility {
			normal_mem_inner_non_cach = 0b00,                                     /**< Normal memory, Non-cacheable */
			normal_mem_inner_write_back_read_alloc_write_alloc_cach = 0b01,       /**< Normal memory, Write-Back Read-Allocate Write-Allocate Cacheable */
			normal_mem_inner_write_through_read_alloc_no_write_alloc_cach = 0b10, /**< Normal memory, Write-Through Read-Allocate No Write-Allocate Cacheable */
			normal_mem_inner_write_back_read_alloc_no_write_alloc_cach = 0b11,    /**< Normal memory, Write-Back Read-Allocate No Write-Allocate Cacheable */
		};

		/**
		 * @enum sharibility
		 * @brief Sharibility Settings
		 */
		enum sharibility {
			non_shareable = 0b00,   /**< Non-shareable */
			outer_shareable = 0b10, /**< Outer Shareable */
			inner_shareable = 0b11, /**< Inner Shareable */
		};

		/**
		 * @enum granuleSize
		 * @brief Granule size
		 */
		enum granuleSize {
			size_4k,
			size_16k,
			size_64k,
		};

		/**
		 * @fn TCR
		 * @brief Read current TCR register and construct TCR object
		 */
		TCR();

		TCR(const TCR& other) = delete;

		TCR(TCR&& other) = delete;

		TCR& operator=(const TCR& other) = delete;

		TCR& operator=(TCR&& other) = delete;

		/**
		 * @fn void reset()
		 * @brief Reset to zero
		 */
		void reset();

		/**
		 * @fn size_t getTTBR0RegionSize() const
		 * @brief Get region size of TTBR0 in bytes
		 */
		size_t getTTBR0RegionSize() const;

		/**
		 * @fn void setTTBR0RegionSize(size_t size)
		 * @brief Set region size of TTBR0 in bytes
		 */
		void setTTBR0RegionSize(size_t size);

		/**
		 * @fn bool isTTBR0TableWalkDisabled() const
		 * @brief Check if Translation Table Walk for TTBR0 is disabled
		 */
		bool isTTBR0TableWalkDisabled() const;

		/**
		 * @fn void setTTBR0TableWalkDisabled(bool disabled)
		 * @brief Set Translation Table Walk for TTBR0 disabled
		 */
		void setTTBR0TableWalkDisabled(bool disabled);

		/**
		 * @fn cachebility getTTBR0InnerCach() const
		 * @brief Get Inner Cachebility for table memory of TTBR0
		 */
		cachebility getTTBR0InnerCach() const;

		/**
		 * @fn void setTTBR0InnerCach(cachebility val)
		 * @brief Set Inner Cachebility for table memory of TTBR0
		 */
		void setTTBR0InnerCach(cachebility val);

		/**
		 * @fn cachebility getTTBR0OuterCach() const
		 * @brief Get Outer Cachebility for table memory of TTBR0
		 */
		cachebility getTTBR0OuterCach() const;

		/**
		 * @fn void setTTBR0OuterCach(cachebility val)
		 * @brief Set Outer Cachebility for table memory of TTBR0
		 */
		void setTTBR0OuterCach(cachebility val);

		/**
		 * @fn sharibility getTTBR0Sharibility() const
		 * @brief Get Shareability for table memory of TTBR0
		 */
		sharibility getTTBR0Sharibility() const;

		/**
		 * @fn void setTTBR0Sharibility(sharibility val) const
		 * @brief Set Shareability for table memory of TTBR0
		 */
		void setTTBR0Sharibility(sharibility val);

		/**
		 * @fn granuleSize getTTBR0GranuleSize() const
		 * @brief get Granule Size for TTBR0
		 */
		granuleSize getTTBR0GranuleSize() const;

		/**
		 * @fn void setTTBR0GranuleSize(granuleSize val)
		 * @brief Set Granule Size for TTBR0
		 */
		void setTTBR0GranuleSize(granuleSize val);

		/**
		 * @fn size_t getTTBR1RegionSize() const
		 * @brief Get region size of TTBR1 in bytes
		 */
		size_t getTTBR1RegionSize() const;

		/**
		 * @fn void setTTBR1RegionSize(size_t size)
		 * @brief Set region size of TTBR1 in bytes
		 */
		void setTTBR1RegionSize(size_t size);

		/**
		 * @fn bool isTTBR1TableWalkDisabled() const
		 * @brief Check if Translation Table Walk for TTBR1 is disabled
		 */
		bool isTTBR1TableWalkDisabled() const;

		/**
		 * @fn void setTTBR1TableWalkDisabled(bool disabled)
		 * @brief Set Translation Table Walk for TTBR1 disabled
		 */
		void setTTBR1TableWalkDisabled(bool disabled);

		/**
		 * @fn cachebility getTTBR1InnerCach() const
		 * @brief Get Inner Cachebility for table memory of TTBR1
		 */
		cachebility getTTBR1InnerCach() const;

		/**
		 * @fn void setTTBR1InnerCach(cachebility val)
		 * @brief Set Inner Cachebility for table memory of TTBR1
		 */
		void setTTBR1InnerCach(cachebility val);

		/**
		 * @fn cachebility getTTBR1OuterCach() const
		 * @brief Get Outer Cachebility for table memory of TTBR1
		 */
		cachebility getTTBR1OuterCach() const;

		/**
		 * @fn void setTTBR1OuterCach(cachebility val)
		 * @brief Set Outer Cachebility for table memory of TTBR1
		 */
		void setTTBR1OuterCach(cachebility val);

		/**
		 * @fn sharibility getTTBR1Sharibility() const
		 * @brief Get Shareability for table memory of TTBR1
		 */
		sharibility getTTBR1Sharibility() const;

		/**
		 * @fn void setTTBR1Sharibility(sharibility val)
		 * @brief Set Shareability for table memory of TTBR1
		 */
		void setTTBR1Sharibility(sharibility val);

		/**
		 * @fn granuleSize getTTBR1GranuleSize() const
		 * @brief get Granule Size for TTBR1
		 */
		granuleSize getTTBR1GranuleSize() const;

		/**
		 * @fn void setTTBR1GranuleSize(granuleSize val)
		 * @brief Set Granule Size for TTBR1
		 */
		void setTTBR1GranuleSize(granuleSize val);

		/**
		 * @fn void useDefaultSetting()
		 * @brief Use default setting
		 * @details
		 * In the default setting, the TCR will utilize (1 << 48) bytes for
		 * TTBR0/TTRB1 region space while use 4K pages as its default page
		 * size.
		 */
		void useDefaultSetting();
};

} /* namespace hw::reg */

#endif /* ifndef _INC_HW_REGISTER_TCR_H_ */
