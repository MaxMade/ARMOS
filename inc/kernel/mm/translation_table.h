#ifndef _INC_KERNEL_MM_TRANSLATION_TABLE_H_
#define _INC_KERNEL_MM_TRANSLATION_TABLE_H_

/**
 * @file kernel/mm/translation_table.h
 * @brief Abstraction of Translation Table
 * @see
 * D5.3.1 VMSAv8-64 translation table level 0, level 1, and level 2 descriptor formats
 * D5.3.2 Armv8 translation table level 3 descriptor formats
 * D5.4.10 Software management of the Access flag
 * Arm Architecture Reference Manual Armv8, for Armv8-A architecture profile
 */

#include <cstdint.h>
#include <cstddef.h>

namespace mm {

	class TranslationTable {
		private:

			typedef union {
				struct {
					uint64_t present:1;    /**< Bit 00: Present Bit */
					uint64_t one:1;        /**< Bit 01: Must be one */
					uint64_t attrIndex:3;  /**< Bit 02: Memory Attributes Index Field (see MAIR) */
					uint64_t ns:1;         /**< Bit 05: Non-Secure Bit */
					uint64_t ap:2;         /**< Bit 06: Data Access Permission Bits */
					uint64_t sh:2;         /**< Bit 08: Shareability Field */
					uint64_t aF:1;         /**< Bit 10: Access-Flag Bit*/
					uint64_t nG:1;         /**< Bit 11: Not-Global Bit */
					uint64_t addr:36;      /**< Bit 12: Physical address of next entry */
					uint64_t res:3;        /**< Bit 48: Reserved */
					uint64_t dbm:1;        /**< Bit 51: Dirty Bit Modifier */
					uint64_t contiguous:1; /**< Bit 52: Hint for contiguous set of entries */
					uint64_t pxn:1;        /**< Bit 53: Privileged Execute-Never */
					uint64_t xn:1;         /**< Bit 54: Execute-Never*/
					uint64_t ignored:9;    /**< Bit 55: Ignored */
				};

				uint64_t value;
			} tt_format_t;

			tt_format_t* addr;

		public:

			/**
			 * @var NUM_ENTRIES
			 * @brief Number of entries per translation table
			 */
			static const size_t NUM_ENTRIES = 512;

			/**
			 * @fn TranslationTable()
			 * @brief Create invalid translation table using TranslationTable(nullptr)
			 */
			TranslationTable();

			TranslationTable& operator=(const TranslationTable& other) = default;

			TranslationTable& operator=(TranslationTable&& other) = default;

			/**
			 * @fn TranslationTable(void* addr)
			 * @brief Create translation table from page-aligned address
			 */
			explicit TranslationTable(void* addr);

			/**
			 * @fn bool isValid() const
			 * @brief Check if underlying address is page-aligned and non null
			 */
			bool isValid() const;

			/**
			 * @fn void* getFrame() const
			 * @breif Return pointer to underlying pointer
			 */
			void* getFrame() const;

			/**
			 * @fn int updateTTBR0() const
			 * @brief Use the translation table as current TTBR0 entry if valid
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int updateTTBR0() const;

			/**
			 * @fn void reset()
			 * @brief Reset whole table to zero
			 */
			void reset();

			/**
			 * @fn int reset(size_t entry)
			 * @brief Reset entry to zero
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int reset(size_t entry);

			/**
			 * @fn int setDefault(size_t entry)
			 * @brief Set entry to default value
			 * @warning entry must be less than NUM_ENTRIES
			 * @details
			 * - present:   0
			 * - attrIndex: 1 (Normal memory)
			 * - ns:        0
			 * - ap:        1 (ELX_RW_EL0_RW)
			 * - sh:        0 (NON_SHAREABLE)
			 * - aF:        1 (Software Access Flag see D5.4.10)
			 * - nG:        0
			 * - addr:      0
			 * - dbm:       0
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int setDefault(size_t entry);

			/**
			 * @fn void setDefault()
			 * @brief Set table to default value
			 * @details
			 * - present:   0
			 * - attrIndex: 1 (Normal memory)
			 * - ns:        0
			 * - ap:        1 (ELX_RW_EL0_RW)
			 * - sh:        0 (NON_SHAREABLE)
			 * - aF:        1 (Software Access Flag see D5.4.10)
			 * - nG:        0
			 * - addr:      0
			 * - dbm:       0
			 */
			void setDefault();

			/**
			 * @fn int getPresentBit(size_t entry) const
			 * @brief Get present bit
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  1 - Entry is present
			 *	-  0 - Entry is non-present
			 *	- <1 - Failure (-errno)
			 */
			int getPresentBit(size_t entry) const;

			/**
			 * @fn int setPresentBit(size_t entry, bool present)
			 * @brief Set entry as present or not
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int setPresentBit(size_t entry, bool present);

			/**
			 * @fn int getAttrIndex(size_t entry) const
			 * @brief Get index (in MAIR) for entry
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	- [7:0] - Success
			 *	- <1    - Failure (-errno)
			 */
			int getAttrIndex(size_t entry) const;

			/**
			 * @fn int setAttrIndex(size_t entry, size_t index)
			 * @brief Set index (in MAIR) for entry
			 * @warning entry must be less than NUM_ENTRIES
			 * @warning index must be less than 8
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int setAttrIndex(size_t entry, size_t index);

			/**
			 * @fn int getNonSecureBit(size_t entry) const
			 * @brief Check if entry is secure or non-secure
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  1 - Entry is non-secure
			 *	-  0 - Entry is secure
			 *	- <1 - Failure (-errno)
			 */
			int getNonSecureBit(size_t entry) const;

			/**
			 * @fn int setNonSecureBit(size_t entry, bool non_secure)
			 * @brief Set entry as secure or non-secure
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int setNonSecureBit(size_t entry, bool non_secure);

			/**
			 * @typedef access_t
			 * @brief Access Control
			 */
			typedef enum access {
				ELX_RW_EL0_NONE = 0b00, /**< Privileged: RW or User: None */
				ELX_RW_EL0_RW   = 0b01, /**< Privileged: RW or User: RW */
				ELX_RO_EL0_NONE = 0b10, /**< Privileged: RO or User: None */
				ELX_RO_EL0_RO   = 0b11, /**< Privileged: RO or User: RO */
			} access_t;

			/**
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int getAccessControl(size_t entry) const;

			/**
			 * @fn int setAccessControl(size_t entry, access_t access)
			 * @brief Set access cotnrol for entry
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int setAccessControl(size_t entry, access_t access);

			/**
			 * @typedef shareability_t
			 * @brief Shareability
			 */
			typedef enum {
				NON_SHAREABLE   = 0b00, /**< Non shareable */
				OUTER_SHARABLE  = 0b10, /**< Outer shareable */
				INNER_SHAREABLE = 0b11, /**< Inner shareable */
			} shareability_t;

			/**
			 * @fn int getShareability(size_t entry) const
			 * @brief Get shareability for entry
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	- [NON_SHAREABLE, OUTER_SHARABLE, INNER_SHAREABLE] - Success
			 *	- <1                                               - Failure (-errno)
			 */
			int getShareability(size_t entry) const;

			/**
			 * @fn int setShareability(size_t entry, shareability_t shareability)
			 * @brief Set shareability for entry
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int setShareability(size_t entry, shareability_t shareability);

			/**
			 * @fn int getAccessedBit(size_t entry) const
			 * @brief Check if entry is accessed or not
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  1 - Entry is accessed
			 *	-  0 - Entry is not accessed
			 *	- <1 - Failure (-errno)
			 */
			int getAccessedBit(size_t entry) const;

			/**
			 * @fn int setAccessedBit(size_t entry, bool accessed)
			 * @brief Set entry as accessed or not
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int setAccessedBit(size_t entry, bool accessed);

			/**
			 * @fn int getNotGlobalBit(size_t entry) const
			 * @brief Check if is not global
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  1 - Entry is not global
			 *	-  0 - Entry is global
			 *	- <1 - Failure (-errno)
			 */
			int getNotGlobalBit(size_t entry) const;

			/**
			 * @fn int setNotGlobalBit(size_t entry, bool not_global)
			 * @brief Set entry as global or not
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int setNotGlobalBit(size_t entry, bool not_global);

			/**
			 * @fn int getPrivilegedExecuteNeverBit(size_t entry) const
			 * @brief Check if entry is privileged-execute-never
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  1 - Entry is pxn
			 *	-  0 - Entry is not pxn
			 *	- <1 - Failure (-errno)
			 */
			int getPrivilegedExecuteNeverBit(size_t entry) const;

			/**
			 * @fn int setPrivilegedExecuteNeverBit(size_t entry, bool pxn)
			 * @brief Set entry as privileged-execute-never or not
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int setPrivilegedExecuteNeverBit(size_t entry, bool pxn);

			/**
			 * @brief int getExecuteNeverBit(size_t entry) const
			 * @brief Check if entry is execute-never
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  1 - Entry is execute-never
			 *	-  0 - Entry is not execute-never
			 *	- <1 - Failure (-errno)
			 */
			int getExecuteNeverBit(size_t entry) const;

			/**
			 * @int setExecuteNeverBit(size_t entry, bool execute_never)
			 * @brief Set entry as execute-never or not
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  0 - Success
			 *	- <1 - Failure (-errno)
			 */
			int setExecuteNeverBit(size_t entry, bool execute_never);

			/**
			 * @fn void* getAddress(size_t entry) const
			 * @brief Get (page-aligned) address for entry
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	- Pointer to next entry   - Success
			 *	- makeError<void*>(errno) - Failure
			 */
			void* getAddress(size_t entry) const;

			/**
			 * @fn int setAddress(size_t entry, void* addr)
			 * @brief Set (page-aligned) address for entry
			 * @warning entry must be less than NUM_ENTRIES
			 * @return
			 *
			 *	-  0 - Success
			 *	- <0 - Failure (-errno)
			 */
			int setAddress(size_t entry, void* addr);
	};

} /* namespace mm */

#endif /* ifndef _INC_KERNEL_MM_TRANSLATION_TABLE_H_ */
