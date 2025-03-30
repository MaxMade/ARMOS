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
			unkown_reson                         = 0b000000, /**< Unknown reason */
			trapped_wfi_wfe                      = 0b000001, /**< Trapped WFI or WFE instruction execution */
			trapped_mcr_mrc_0                    = 0b000011, /**< Trapped MCR or MRC access */
			trapped_mcrr_mrrc                    = 0b000100, /**< Trapped MCRR or MRRC access */
			trapped_mcr_mrc_1                    = 0b000101, /**< Trapped MCR or MRC access */
			trapped_ldc_stc                      = 0b000110, /**< Trapped LDC or STC access */
			trapped_fp                           = 0b000111, /**< Trapped access to SVE, Advanced SIMD, or floating-point functionality */
			trapped_mrrc                         = 0b001100, /**< Trapped MRRC access */
			brnach_target_exception              = 0b001101, /**< Branch target exception */
			illegal_execution_state              = 0b001110, /**< Illegal Execution state */
			trapped_svc_aarch32                  = 0b010001, /**< SVC instruction execution in AArch32 state */
			trapped_svc_aarch64                  = 0b010101, /**< SVC instruction execution in AArch64 state */
			trapped_msr_mrs_system_instr_aarch64 = 0b011000, /**< Trapped MSR, MRS or System instruction execution in AArch64 state */
			trapped_sve                          = 0b011001, /**< Access to SVE functionality trapped as a result of CPACR_EL1.ZEN */
			trapped_pac                          = 0b011100, /**< Exception from a Pointer Authentication instruction authentication failure */
			instruction_abort_with_el_change     = 0b100000, /**< Instruction Abort from a lower Exception level (Pagefault during instruction access) */
			instruction_abort                    = 0b100001, /**< Instruction Abort taken without a change in Exception level (Pagefault during instruction access) */
			pc_alignment_fault                   = 0b100010, /**< PC alignment fault exception */
			data_abort_with_el_change            = 0b100100, /**< Data Abort from a lower Exception level (Pagefault during data access) */
			data_abort                           = 0b100101, /**< Data Abort taken without a change in Exception level (Pagefault during data access) */
			sp_alignment_fault                   = 0b100110, /**< SP alignment fault exception */
			trapped_fp_aarch32                   = 0b101000, /**< Trapped floating-point exception taken from AArch32 state */
			trapped_fp_aarch64                   = 0b101100, /**< Trapped floating-point exception taken from AArch64 state */
			serror_interrupt                     = 0b101111, /**< SError interrupt */
			breakpoint_with_el_change            = 0b110000, /**< Breakpoint exception from a lower Exception level */
			breakpoint                           = 0b110001, /**< Breakpoint exception taken without a change in Exception level */
			sw_step_exception_with_el_change     = 0b110010, /**< Software Step exception from a lower Exception level */
			sw_step_exception                    = 0b110011, /**< Software Step exception taken without a change in Exception level */
			watchpoint_with_el_change            = 0b110100, /**< Watchpoint exception from a lower Exception level */
			watchpoint                           = 0b110101, /**< Watchpoint exception taken without a change in Exception level */
			bkpt_aarch32                         = 0b111000, /**< BKPT instruction execution in AArch32 state */
			brk_aarch64                          = 0b111100 /**< BRK instruction execution in AArch64 state */
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
};

} /* namespace hw::reg */

#endif /* ifndef _INC_HW_REGISTER_ESR_H_ */
