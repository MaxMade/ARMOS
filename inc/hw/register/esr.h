#ifndef _INC_HW_REGISTER_ESR_H_
#define _INC_HW_REGISTER_ESR_H_

#include <cstdint.h>

/**
 * @file hw/register/esr.h
 * @brief Exception Syndrome Register (EL1)
 * @see
 * D13.2.36 ESR_EL1, Exception Syndrome Register (EL1)
 * Arm Architecture Reference Manual Armv8, for Armv8-A architecture profile
 */

namespace hw::reg {

/**
 * @class ESR
 * @brief Abstraction of ESR
 */
class ESR {
	private:
		typedef union esr {
			struct {
				uint64_t iss:25; /**< Instruction specific syndrome */
				uint64_t il:1;   /**< Instruction Length for synchronous exceptions */
				uint64_t ec:6;   /**< Exception class */
				uint64_t res:32; /**< Reserved */
			};
			uint64_t value;
		} esr_t;

		/**
		 * @var value
		 * @brief Underlying value of ESR
		 */
		esr_t value;

		/**
		 * @fn void read()
		 * @brief Read current value of ESR into value
		 */
		void read();

	public:
		/**
		 * @fn ESR
		 * @brief Read current ESR register and construct ESR object
		 */
		ESR();

		ESR(const ESR& other) = delete;

		ESR(ESR&& other) = delete;

		ESR& operator=(const ESR& other) = delete;

		ESR& operator=(ESR&& other) = delete;

		/**
		 * @enum ExceptionClass
		 * @brief Overview of all supported exception classes
		 */
		enum ExceptionClass : uint32_t {
			UNKOWN_RESON                         = 0b000000, /**< Unknown reason */
			TRAPPED_WFI_WFE                      = 0b000001, /**< Trapped WFI or WFE instruction execution */
			TRAPPED_MCR_MRC_COPROC_1111          = 0b000011, /**< Trapped MCR or MRC access (coproc==0b1111) */
			TRAPPED_MCRR_MRRC_COPROC_1111        = 0b000100, /**< Trapped MCRR or MRRC access (coproc==0b1111) */
			TRAPPED_MCR_MRC_COPROC_1110          = 0b000101, /**< Trapped MCR or MRC access (coproc==0b1110) */
			TRAPPED_LDC_STC                      = 0b000110, /**< Trapped LDC or STC access */
			TRAPPED_FP                           = 0b000111, /**< Trapped access to SVE, Advanced SIMD, or floating-point functionality */
			TRAPPED_LD64B_ST64B_ST64BV_ST64BV0   = 0b001010, /**< Trapped execution of an LD64B, ST64B, ST64BV, or ST64BV0 instruction */
			TRAPPED_MRRC_COPROC_1110             = 0b001100, /**< Trapped MRRC access (coproc==0b1110) */
			BRANCH_TARGET_EXCEPTION              = 0b001101, /**< Branch target exception */
			ILLEGAL_EXECUTION_STATE              = 0b001110, /**< Illegal Execution state */
			TRAPPED_SVC_AARCH32                  = 0b010001, /**< SVC instruction execution in AArch32 state */
			TRAPPED_SVC_AARCH64                  = 0b010101, /**< SVC instruction execution in AArch64 state */
			TRAPPED_MSR_MRS_SYSTEM_INSTR_AARCH64 = 0b011000, /**< Trapped MSR, MRS or System instruction execution in AArch64 state */
			TRAPPED_SVE                          = 0b011001, /**< Access to SVE functionality trapped as a result of CPACR_EL1.ZEN */
			TRAPPED_PAC                          = 0b011100, /**< Exception from a Pointer Authentication instruction authentication failure */
			INSTRUCTION_ABORT_WITH_EL_CHANGE     = 0b100000, /**< Instruction Abort from a lower Exception level (Pagefault during instruction access) */
			INSTRUCTION_ABORT                    = 0b100001, /**< Instruction Abort taken without a change in Exception level (Pagefault during instruction access) */
			PC_ALIGNMENT_FAULT                   = 0b100010, /**< PC alignment fault exception */
			DATA_ABORT_WITH_EL_CHANGE            = 0b100100, /**< Data Abort from a lower Exception level (Pagefault during data access) */
			DATA_ABORT                           = 0b100101, /**< Data Abort taken without a change in Exception level (Pagefault during data access) */
			SP_ALIGNMENT_FAULT                   = 0b100110, /**< SP alignment fault exception */
			TRAPPED_FP_AARCH32                   = 0b101000, /**< Trapped floating-point exception taken from AArch32 state */
			TRAPPED_FP_AARCH64                   = 0b101100, /**< Trapped floating-point exception taken from AArch64 state */
			SERROR_INTERRUPT                     = 0b101111, /**< SError interrupt */
			BREAKPOINT_WITH_EL_CHANGE            = 0b110000, /**< Breakpoint exception from a lower Exception level */
			BREAKPOINT                           = 0b110001, /**< Breakpoint exception taken without a change in Exception level */
			SW_STEP_EXCEPTION_WITH_EL_CHANGE     = 0b110010, /**< Software Step exception from a lower Exception level */
			SW_STEP_EXCEPTION                    = 0b110011, /**< Software Step exception taken without a change in Exception level */
			WATCHPOINT_WITH_EL_CHANGE            = 0b110100, /**< Watchpoint exception from a lower Exception level */
			WATCHPOINT                           = 0b110101, /**< Watchpoint exception taken without a change in Exception level */
			BKPT_AARCH32                         = 0b111000, /**< BKPT instruction execution in AArch32 state */
			BRK_AARCH64                          = 0b111100  /**< BRK instruction execution in AArch64 state */
		};

		/**
		 * @fn uint32_t getISS() const
		 * @brief Get instruction Specific syndrome
		 */
		uint32_t getISS() const;

		/**
		 * @fn uint32_t getIL() const
		 * @brief Get instruction length for synchronous exception field
		 * @return
		 *	- 0 - 16-bit instruction
		 *	- 1 - 32-bit instruction
		 */
		uint32_t getIL() const;

		/**
		 * @fn uint32_t getEC() const
		 * @brief Get exception class
		 */
		uint32_t getEC() const;

		/**
		 * @fn const char* getECString() const
		 * @brief Get exception class as string
		 *
		 *	- Pointer to string - Success
		 *	- nullptr           - Failure
		 */
		const char* getECString() const;

		/**
		 * @enum TI
		 * @brief Trapped Instruction
		 */
		enum class TI : uint64_t {
			WFI  = 0b00, /** WFI */
			WFE  = 0b01, /** WFE */
			WFIT = 0b10, /** WFIT */
			WFET = 0b11, /** WFET */
		};

		/**
		 * @enum CV
		 * @brief Condition-Code Valid
		 */
		enum class CV : uint64_t {
			INVALID = 0b0, /**< Field COND is not valid */
			VALID   = 0b1, /**< Field COND is valid */
		};

		/**
		 * @enum DIRECTION
		 * @brief Direction of trapped instruction
		 */
		enum class DIRECTION : uint64_t {
			WRITE = 0b0, /**< Writing (MCR) */
			READ  = 0b1, /**< Reading (MRC/VMRS) */
		};

		/**
		 * @enum ISS
		 * @brief Trapped instruction
		 */
		enum class ISS : uint64_t {
			ST64BV      = 0b0000000000000000000000000, /**< ST64BV instruction trapped */
			ST64BV0     = 0b0000000000000000000000001, /**< ST64BV0 instruction trapped */
			LD64B_ST64B = 0b0000000000000000000000010, /**< LD64B or ST64B instruction trapped */
		};

		/**
		 * @enum OFFSET
		 * @brief Offset
		 */
		enum class OFFSET : uint64_t {
			SUB = 0b0, /**< Subtracted */
			ADD = 0b1, /**< Added */
		};

		/**
		 * @enum AM
		 * @brief Addressing Mode
		 */
		enum class AM : uint32_t {
			IMM_UNINDEXED   = 0b000, /**< Immediate unindexed */
			IMM_POSTINDEXED = 0b001, /**< Immediate post-indexed */
			IMM_OFFSET      = 0b010, /**< Immediate offset*/
			IMM_PREINDEXED  = 0b011, /**< Immediate pre-indexed */
			UNKOWN0         = 0b100, /**< Unkown */
			UNKOWN1         = 0b110, /**< Unkown */
		};

		/**
		 * @enum CCKNOWNPASS
		 * @brief Condition Code Paseed
		 */
		enum class CCKNOWNPASS : uint64_t {
			PASSED = 0b0, /**< The instruction was unconditional, or was conditional and passed its condition code check */
			FAILED = 0b1, /**< The instruction was conditional */
		};

		/**
		 * @enum SET
		 * @brief Synchronous Error Type
		 */
		enum class SET : uint64_t {
			RECOVERABLE   = 0b00, /**< Uncoverable state (UER) */
			UNCONTAINABLE = 0b10, /**< Uncontainable state (UC) */
			RESTARTABLE   = 0b11, /**< Uestartable state (UEO) */
		};

		/**
		 * @enum FNV
		 * @brief FAR not valid
		 */
		enum class FNV : uint64_t  {
			VALID   = 0b0, /**< Valid */
			INVALID = 0b1, /**< Invalid */
		};

		/**
		 * @enum S1PTW
		 * @brief Stage 2 fault for stage 1 table walk
		 */
		enum class S1PTW : uint64_t {
			NO_STAGE2_FAULT = 0b0, /**< Fault not on a stage 2 translation for a stage 1 translation table walk */
			STAGE2_FAULT    = 0b1, /**< Fault on a stage 2 translation for a stage 1 translation table walk */
		};

		/**
		 * @enum IFSC
		 * @brief Instruction Fault Status Code
		 */
		enum class IFSC : uint64_t {
			ADDRESS_SIZE_FAULT_LVL_0           = 0b000000, /**< Address size fault, level 0 of translation or translation table base register */
			ADDRESS_SIZE_FAULT_LVL_1           = 0b000001, /**< Address size fault, level 1 */
			ADDRESS_SIZE_FAULT_LVL_2           = 0b000010, /**< Address size fault, level 2 */
			ADDRESS_SIZE_FAULT_LVL_3           = 0b000011, /**< Address size fault, level 3 */
			TRANSLATION_FAULT_LVL_0            = 0b000100, /**< Translation fault, level 0 */
			TRANSLATION_FAULT_LVL_1            = 0b000101, /**< Translation fault, level 1 */
			TRANSLATION_FAULT_LVL_2            = 0b000110, /**< Translation fault, level 2 */
			TRANSLATION_FAULT_LVL_3            = 0b000111, /**< Translation fault, level 3 */
			ACCESS_FLAG_FAULT_LVL_1            = 0b001001, /**< Access flag fault, level 1 */
			ACCESS_FLAG_FAULT_LVL_2            = 0b001010, /**< Access flag fault, level 2 */
			ACCESS_FLAG_FAULT_LVL_3            = 0b001011, /**< Access flag fault, level 3 */
			ACCESS_FLAG_FAULT_LVL_0            = 0b001000, /**< Access flag fault, level 0 */
			PERMISSION_FAULT_0                 = 0b001100, /**< Permission fault, level 0 */
			PERMISSION_FAULT_1                 = 0b001101, /**< Permission fault, level 1 */
			PERMISSION_FAULT_2                 = 0b001110, /**< Permission fault, level 2 */
			PERMISSION_FAULT_3                 = 0b001111, /**< Permission fault, level 3 */
			SYNC_EXT_ABORT                     = 0b010000, /**< Synchronous External abort, not on translation table walk or hardware update of translation table */
			SYNC_EXT_ABOR_LVL_NEG_1            = 0b010011, /**< Synchronous External abort on translation table walk or hardware update of translation table, level -1 */
			SYNC_EXT_ABOR_LVL_0                = 0b010100, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 0 */
			SYNC_EXT_ABOR_LVL_1                = 0b010101, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 1 */
			SYNC_EXT_ABOR_LVL_2                = 0b010110, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 2 */
			SYNC_EXT_ABOR_LVL_3                = 0b010111, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 3 */
			SYNC_PARITY_OR_ECC_ERROR           = 0b011000, /**< Synchronous parity or ECC error on memory access, not on translation table walk */
			SYNC_PARITY_OR_ECC_ERROR_LVL_NEG_1 = 0b011011, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level -1 */
			SYNC_PARITY_OR_ECC_ERROR_LVL_0     = 0b011100, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 0 */
			SYNC_PARITY_OR_ECC_ERROR_LVL_1     = 0b011101, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 1 */
			SYNC_PARITY_OR_ECC_ERROR_LVL_2     = 0b011110, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 2 */
			SYNC_PARITY_OR_ECC_ERROR_LVL_3     = 0b011111, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 3 */

			ADDRESS_SIZE_FAULT_LVL_NEG_1       = 0b101001, /**< Address size fault, level -1 */
			TRANSLATION_FAULT_LVL_NEG_1        = 0b101011, /**< Translation fault, level -1 */
			TLB_CONFLICT_ABORT                 = 0b110000, /**< TLB conflict abort */
			UNSUPPORTED_ATOMIC_UPDATE          = 0b110001, /**< Unsupported atomic hardware update fault */
		};

		/**
		 * @enum ISV
		 * @brief Instruction Syndrome Valid
		 */
		enum class ISV : uint64_t {
			INVALID = 0b0, /**< No valid instruction syndrome */
			VALID   = 0b1, /**< Valid instruction syndrome */
		};

		/**
		 * @enum SAS
		 * @brief Syndrome Access Size
		 */
		enum class SAS : uint64_t {
			BYTE       = 0b00, /**< Byte */
			HALFWORD   = 0b01, /**< Halfword */
			WORD       = 0b10, /**< Word */
			DOUBLEWORD = 0b11, /**< Doubleword */
		};

		/**
		 * @enum SSE
		 * @brief Syndrome Sign Extend
		 */
		enum class SSE : uint64_t {
			EXTEND    = 0b0, /**< Sign-extension not required */
			NO_EXTEND = 0b1, /**< Sing-extension required */
		};

		/**
		 * @enum SF
		 * @brief Sixty-Four Register Access
		 */
		enum class SF : uint64_t {
			REG32 = 0b0, /**< Instruction loads/stores a 32-bit wide register */
			REG64 = 0b1, /**< Instruction loads/stores a 64-bit wide register */
		};

		/**
		 * @enum AR
		 * @brief Aquire Release Semantic
		 */
		enum class AR : uint64_t {
			NO  = 0b0, /**< Instruction did not have acquire/release semantics */
			YES = 0b1, /**< Instruction did have acquire/release semantics */
		};

		/**
		 * @enum LST
		 * @brief Load/Store Type
		 */
		enum class LST : uint64_t {
			ST64BV       = 0b01, /**< An ST64BV instruction generated the Data Abort */
			LD64B_ST64BV = 0b10, /**< An LD64B or ST64B instruction generated the Data Abort */
			ST64BV0      = 0b11, /**< An ST64BV0 instruction generated the Data Abort */
		};

		/**
		 * @enum WNR
		 * @brief Write not Read
		 */
		enum class WNR : uint64_t {
			READ  = 0b0, /**< Abort caused by reading instruction */
			WRITE = 0b1, /**< Abort caused by writing instruction */
		};

		/**
		 * @enum DFSC
		 * @brief Data Fault Status Code
		 */
		enum class DFSC : uint64_t {
			ADDRESS_SIZE_FAULT_LVL_0           = 0b000000, /**< Address size fault, level 0 of translation or translation table base register */
			ADDRESS_SIZE_FAULT_LVL_1           = 0b000001, /**< Address size fault, level 1 */
			ADDRESS_SIZE_FAULT_LVL_2           = 0b000010, /**< Address size fault, level 2 */
			ADDRESS_SIZE_FAULT_LVL_3           = 0b000011, /**< Address size fault, level 3 */
			TRANSLATION_FAULT_LVL_0            = 0b000100, /**< Translation fault, level 0 */
			TRANSLATION_FAULT_LVL_1            = 0b000101, /**< Translation fault, level 1 */
			TRANSLATION_FAULT_LVL_2            = 0b000110, /**< Translation fault, level 2 */
			TRANSLATION_FAULT_LVL_3            = 0b000111, /**< Translation fault, level 3 */
			ACCESS_FLAG_FAULT_LVL_1            = 0b001001, /**< Access flag fault, level 1 */
			ACCESS_FLAG_FAULT_LVL_2            = 0b001010, /**< Access flag fault, level 2 */
			ACCESS_FLAG_FAULT_LVL_3            = 0b001011, /**< Access flag fault, level 3 */
			ACCESS_FLAG_FAULT_LVL_0            = 0b001000, /**< Access flag fault, level 0 */
			PERMISSION_FAULT_0                 = 0b001100, /**< Permission fault, level 0 */
			PERMISSION_FAULT_1                 = 0b001101, /**< Permission fault, level 1 */
			PERMISSION_FAULT_2                 = 0b001110, /**< Permission fault, level 2 */
			PERMISSION_FAULT_3                 = 0b001111, /**< Permission fault, level 3 */
			SYNC_EXT_ABORT                     = 0b010000, /**< Synchronous External abort, not on translation table walk or hardware update of translation table */
			SYNC_EXT_ABOR_LVL_NEG_1            = 0b010011, /**< Synchronous External abort on translation table walk or hardware update of translation table, level -1 */
			SYNC_EXT_ABOR_LVL_0                = 0b010100, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 0 */
			SYNC_EXT_ABOR_LVL_1                = 0b010101, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 1 */
			SYNC_EXT_ABOR_LVL_2                = 0b010110, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 2 */
			SYNC_EXT_ABOR_LVL_3                = 0b010111, /**< Synchronous External abort on translation table walk or hardware update of translation table, level 3 */
			SYNC_PARITY_OR_ECC_ERROR           = 0b011000, /**< Synchronous parity or ECC error on memory access, not on translation table walk */
			SYNC_PARITY_OR_ECC_ERROR_LVL_NEG_1 = 0b011011, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level -1 */
			SYNC_PARITY_OR_ECC_ERROR_LVL_0     = 0b011100, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 0 */
			SYNC_PARITY_OR_ECC_ERROR_LVL_1     = 0b011101, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 1 */
			SYNC_PARITY_OR_ECC_ERROR_LVL_2     = 0b011110, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 2 */
			SYNC_PARITY_OR_ECC_ERROR_LVL_3     = 0b011111, /**< Synchronous parity or ECC error on memory access on translation table walk or hardware update of translation table, level 3 */
			ADDRESS_SIZE_FAULT_LVL_NEG_1       = 0b101001, /**< Address size fault, level -1 */
			TRANSLATION_FAULT_LVL_NEG_1        = 0b101011, /**< Translation fault, level -1 */
			TLB_CONFLICT_ABORT                 = 0b110000, /**< TLB conflict abort */
			UNSUPPORTED_ATOMIC_UPDATE          = 0b110001, /**< Unsupported atomic hardware update fault */
		};

		/**
		 * @enum CM
		 * @brief Cache Maintenace
		 */
		enum class CM : uint64_t {
			NO  = 0b0, /**< Fault was not caused by cache maintenace */
			YES = 0b1, /**< Fault was caused by cache maintenace */
		};

		/**
		 * @enum EA
		 * @brief External Abort
		 */
		enum class EA : uint64_t {
			NO  = 0b0, /**< Fault was not caused by external abort */
			YES = 0b0, /**< Fault was caused by external abort */
		};

		/**
		 * @enum VNCR
		 * @brief Trapped access of VNCR_EL2
		 */
		enum VNCR : uint64_t {
			NO  = 0b0, /**< Fault was not generated by use of VNCR_EL2 */
			YES = 0b1, /**< Fault was generated by use of VNCR_EL2 */
		};

		/**
		 * @enum SRT
		 * @brief Syndrom Register Transfer
		 */
		enum SRT : uint64_t {
			x0 = 0b00000,  /**< Register x0 */
			x1 = 0b00001,  /**< Register x1 */
			x2 = 0b00010,  /**< Register x2 */
			x3 = 0b00011,  /**< Register x3 */
			x4 = 0b00100,  /**< Register x4 */
			x5 = 0b00101,  /**< Register x5 */
			x6 = 0b00110,  /**< Register x6 */
			x7 = 0b00111,  /**< Register x7 */
			x8 = 0b01000,  /**< Register x8 */
			x9 = 0b01001,  /**< Register x9 */
			x10 = 0b01010, /**< Register x10 */
			x11 = 0b01011, /**< Register x11 */
			x12 = 0b01100, /**< Register x12 */
			x13 = 0b01101, /**< Register x13 */
			x14 = 0b01110, /**< Register x14 */
			x15 = 0b01111, /**< Register x15 */
			x16 = 0b10000, /**< Register x16 */
			x17 = 0b10001, /**< Register x17 */
			x18 = 0b10010, /**< Register x18 */
			x19 = 0b10011, /**< Register x19 */
			x20 = 0b10100, /**< Register x20 */
			x21 = 0b10101, /**< Register x21 */
			x22 = 0b10110, /**< Register x22 */
			x23 = 0b10111, /**< Register x23 */
			x24 = 0b11000, /**< Register x24 */
			x25 = 0b11001, /**< Register x25 */
			x26 = 0b11010, /**< Register x26 */
			x27 = 0b11011, /**< Register x27 */
			x28 = 0b11100, /**< Register x28 */
			x29 = 0b11101, /**< Register x29 */
			x30 = 0b11110, /**< Register x30 */
			x31 = 0b11111, /**< Register x31 */
		};

		/**
		 * @enum COND
		 * @brief Condition
		 */
		enum class COND : uint64_t {
			VALID = 0b1110, /**< AARCH64 or AARCH32 unconditional */
		};

		/**
		 * @union DataAbort
		 * @brief Data Abort caused by DATA_ABORT or DATA_ABORT_WITH_EL_CHANGE
		 * @warning If ISV is INVALID, then SAS, SSE, SRT, SF, AR are reserved
		 */
		union DataAbort {
			struct {
				DFSC dfsc:6;   /**< Data Fault Status Code [5:0] */
				WNR wnr:1;     /**< Write Not Read [6] */
				S1PTW s1ptw:1; /**< Stage 2 fault for Stage 1 table walk [7] */
				CM cm:1;       /**< Cache Maintenace [8] */
				EA ea:1;       /**< External Abort [9] */
				FNV fnv:1;     /**< FAR not Valid [10] */
				SET set:2;     /**< Synchronous Error Type [12:11] */
				VNCR vncr:1;   /**< Caused by VNCR_EL2 [13] */
				AR ar:1;       /**< Aquire Release [14] */
				SF sf:1;       /**< Sixty-Four Register [15] */
				SRT srt:5;     /**< Syndrome Register Transfer [20:16] */
				SSE see:1;     /**< Syndrome Sign Extend [21] */
				SAS sas:2;     /**< Syndrome Access Size [23:22] */
				ISV isv:1;     /**< Instruction Syndrome Valid [24] */
			};
			uint64_t value;
		};

		/**
		 * @fn int parse_data_abort_with_el_change(DataAbort& dataAbort) const
		 * @brief Parse Data Abort with Exception Level Change
		 * @return
		 *
		 *	-  0 - Success
		 *	- <0 - Failure (-errno)
		 */
		int parse_data_abort_with_el_change(DataAbort& dataAbort) const;

		/**
		 * @fn int parse_data_abort(DataAbort& dataAbort) const
		 * @brief Parse Data Abort
		 * @return
		 *
		 *	-  0 - Success
		 *	- <0 - Failure (-errno)
		 */
		int parse_data_abort(DataAbort& dataAbort) const;

		/**
		 * @union InstructionAbort
		 * @brief Instruction Abort caused by INSTRUCTION_ABORT or INSTRUCTION_ABORT_WITH_EL_CHANGE
		 */
		union InstructionAbort {
			struct {
				IFSC ifsc:6;          /**< Instruction Fault Status Code [5:0] */
				uint64_t reserved0:1; /**< Reserved [6] */
				S1PTW s1ptw;          /**< Stage 2 fault for Stage 1 table walk [7] */
				uint64_t reserved1:1; /**< Reserved */
				EA ea:1;              /**< External Abort [9] */
				FNV fnv:1;            /**< FAR not Valid [10] */
				SET set:2;            /**< Syncrhonous Error Type [12:11] */
			};
			uint64_t value;
		};

		/**
		 * @fn int parse_instruction_abort_with_el_change(InstructionAbort& instrAbort) const
		 * @brief Parse Instruction Abort caused by INSTRUCTION_ABORT_WITH_EL_CHANGE
		 * @return
		 *
		 *	-  0 - Success
		 *	- <0 - Failure (-errno)
		 */
		int parse_instruction_abort_with_el_change(InstructionAbort& instrAbort) const;

		/**
		 * @fn int parse_instruction_abort(InstructionAbort& instrAbort) const
		 * @brief Parse Instruction Abort caused by INSTRUCTION_ABORT
		 * @return
		 *
		 *	-  0 - Success
		 *	- <0 - Failure (-errno)
		 */
		int parse_instruction_abort(InstructionAbort& instrAbort) const;

		/**
		 * @union Watchpoint
		 * @brief Watchpoint
		 */
		union Watchpoint {
			struct {
				DFSC dfsc:6;          /**< Data Fault Status Code [5:0] */
				WNR wnr:1;            /**< Write not Read [6] */
				uint64_t reserved0:1; /**< Reserved [7] */
				CM cm:1;              /**< Cache Maintenace [8] */
				uint64_t reserved1:4; /**< Reserved [12:9] */
				VNCR vncr:1;          /**< Watchpoint was generated by use of VNCR_EL2 [13] */
			};
			uint64_t value;
		};

		/**
		 * @fn int parse_watchpoint_with_el_change(Watchpoint& watchpoint) const
		 * @brief Parse Watchpoint caused by WATCHPOINT_WITH_EL_CHANGE
		 * @return
		 *
		 *	-  0 - Success
		 *	- <0 - Failure (-errno)
		 */
		int parse_watchpoint_with_el_change(Watchpoint& watchpoint) const;

		/**
		 * @fn int parse_watchpoint(Watchpoint& watchpoint) const
		 * @brief Parse Watchpoint caused by WATCHPOINT
		 * @return
		 *
		 *	-  0 - Success
		 *	- <0 - Failure (-errno)
		 */
		int parse_watchpoint(Watchpoint& watchpoint) const;
};

} /* namespace hw::reg */

#endif /* ifndef _INC_HW_REGISTER_ESR_H_ */
