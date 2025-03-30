#ifndef _INC_HE_REG_PAR_H_
#define _INC_HE_REG_PAR_H_ value

#include <cstdint.h>

/**
 * @file hw/register/par.h
 * @brief Physical Address Register
 */

namespace hw::reg {

	/**
	 * @class PAR
	 * @brief Physical Address Register
	 */
	class PAR {
		private:

			/**
			 * @enum F
			 * @brief Failure Flag
			 */
			enum class F : uint64_t {
				SUCCCESS = 0,
				FAILURE = 1,
			};

			/**
			 * @enum FST
			 * @brief Fault Status Code
			 */
			enum class FST : uint64_t {
				ADDRESS_SIZE_FAULT_LVL_0            = 0b000000, /**< Address size fault, level 0 of translation or translation table base register. */
				ADDRESS_SIZE_FAULT_LVL_1            = 0b000001, /**< Address size fault, level 1. */
				ADDRESS_SIZE_FAULT_LVL_2            = 0b000010, /**< Address size fault, level 2. */
				ADDRESS_SIZE_FAULT_LVL_3            = 0b000011, /**< Address size fault, level 3. */
				TRANSLATION_FAULT_LVL_0             = 0b000100, /**< Translation fault, level 0. */
				TRANSLATION_FAULT_LVL_1             = 0b000101, /**< Translation fault, level 1. */
				TRANSLATION_FAULT_LVL_2             = 0b000110, /**< Translation fault, level 2. */
				TRANSLATION_FAULT_LVL_3             = 0b000111, /**< Translation fault, level 3. */
				ACCESS_FLAG_FAULT_LVL_1             = 0b001001, /**< Access flag fault, level 1. */
				ACCESS_FLAG_FAULT_LVL_2             = 0b001010, /**< Access flag fault, level 2. */
				ACCESS_FLAG_FAULT_LVL_3             = 0b001011, /**< Access flag fault, level 3. */
				ACCESS_FLAG_FAULT_LVL_0             = 0b001000, /**< Access flag fault, level 0. */
				PERMISSION_FAULT_LVL_0              = 0b001100, /**< Permission fault, level 0. */
				PERMISSION_FAULT_LVL_1              = 0b001101, /**< Permission fault, level 1. */
				PERMISSION_FAULT_LVL_2              = 0b001110, /**< Permission fault, level 2. */
				PERMISSION_FAULT_LVL_3              = 0b001111, /**< Permission fault, level 3. */
				SYNCH_EXTERNAL_ABORT_LVL_NEG_1      = 0b010011, /**< Synchronous External abort on translation table walk or hardware update of translation table, level -1. */
				SYNCH_EXTERNAL_ABORT_LVL_0          = 0b010100, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 0. */
				SYNCH_EXTERNAL_ABORT_LVL_1          = 0b010101, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 1. */
				SYNCH_EXTERNAL_ABORT_LVL_2          = 0b010110, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 2. */
				SYNCH_EXTERNAL_ABORT_LVL_3          = 0b010111, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 3. */
				SYNCH_PARITY_OR_ECC_ERROR_LVL_NEG_1 = 0b011011, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level -1. */
				SYNCH_PARITY_OR_ECC_ERROR_LVL_0     = 0b011100, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 0. */
				SYNCH_PARITY_OR_ECC_ERROR_LVL_1     = 0b011101, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 1. */
				SYNCH_PARITY_OR_ECC_ERROR_LVL_2     = 0b011110, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 2. */
				SYNCH_PARITY_OR_ECC_ERROR_LVL_3     = 0b011111, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 3. */
				ADDRESS_SIZE_FAULT_LVL_NEG_1        = 0b101001, /**< Address size fault, level -1. */
				TRANSLATION_FAULT_LVL_NEG_1         = 0b101011, /**< Translation fault, level -1. */
				TLB_CONFLICT_ABORT                  = 0b110000, /**< TLB conflict abort. */
				UNSUPPORTED_ATOMIC_UPDATE_FAULT     = 0b110001, /**< Unsupported atomic hardware update fault. */
				SECTION_DOMAIN_FAULT_AARCH32        = 0b111101, /**< Section Domain fault, from an AArch32 stage 1 EL1&0 translation regime using Short-descriptor translation table format. */
				PAGE_DOMAIN_FAULT_AARCH32           = 0b111110, /**< Page Domain fault, from an AArch32 stage 1 EL1&0 translation regime using Short-descriptor translation table format. */
			};

			/**
			 * @enum SH
			 * @brief Shareability
			 */
			enum class SH : uint64_t {
				NON_SHAREABLE   = 0b00, /**< Non-shareable. */
				OUTER           = 0b10, /**< Outer Shareable. */
				INNER_SHAREABLE = 0b11, /**< Inner Shareable. */
			};

			/**
			 * @enum S
			 * @brief Stage
			 */
			enum class S : uint64_t {
				STAGE_1_FAULT = 0b0, /**< Translation aborted because of a fault in the stage 1 translation */
				STAGE_2_FAULT = 0b1, /**< Translation aborted because of a fault in the stage 2 translation */
			};

			/**
			 * @union ParValid
			 * @brief PAR register (Valid)
			 * @warning f must be SUCCESS
			 */
			union ParValid {
				struct {
					F f:1;           /**< Failure Flag [0] */
					uint64_t res0:6; /**< Reserved [6:1] */
					SH sh:2;         /**< Shareability [8:7] */
					uint64_t res1:3; /**< Reserved [11:9] */
					uint64_t pa:40;  /**< Physical Address [51:12] */
					uint64_t res2:4; /**< Reserved [55:52] */
					uint64_t atrr:8; /**< Attribute (see MAIR) [63:56] */

				};
				uint64_t value;
			};

			/**
			 * @union ParInvalid
			 * @brief PAR register (Invalid)
			 * @warning f must be FAILURE
			 */
			union ParInvalid {
				struct {
					F f:1;            /**< Failure Flag [0] */
					FST fst:6;        /**< Fault Status Code [6:1] */
					uint64_t res0:2;  /**< Reserved [8:7] */
					S s:1;            /**< Stage Abort [9] */
					uint64_t res1:54; /**< Reserved [63:10] */
				};
				uint64_t value;
			};

			/**
			 * @var value
			 * @brief Underlying value
			 */
			uint64_t value;

		public:
			/**
			 * @fn PAR
			 * @brief Read PAR_EL1 in
			 */
			PAR();

			/**
			 * @fn bool isValid() const
			 * @brief Check if PAR.F is SUCCESS
			 */
			bool isValid() const;

			/**
			 * @fn operator bool() const
			 * @brief Check if PAR.F is SUCCESS
			 */
			operator bool() const;

			/**
			 * @fn ParValid getValid() const
			 * @brief Get valid PAR
			 * @warning The PAR object must be check with isValid() before!
			 */
			ParValid getValid() const;

			/**
			 * @fn ParInvalid getInvalid() const
			 * @brief Get invalid PAR
			 * @warning The PAR object must be check with isValid() before!
			 */
			ParInvalid getInvalid() const;
	};

} /* namespace hw::reg */

#endif /* ifndef _INC_HE_REG_PAR_H_ */
