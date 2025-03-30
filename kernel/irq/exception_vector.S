/*
 * Exception vector
 *
 * The ARMv8-A architecture expects an 2k aligned exception vector which
 * consists of 16 entries. Hereby, the vector can be subdivided into 4 smaller
 * sets:
 *
 * 1) Exception taken from current EL with SP_EL0 (Range 0x000 - 0x200)
 * 2) Exception taken from current EL with SP_ELx (Range 0x200 - 0x400)
 * 3) Exception taken from lower EL while using AArch64 (Range 0x400 - 0x600)
 * 3) Exception taken from lower EL while using AArch32 (Range 0x600 - 0x800)
 *
 * Each set contains again 4 entries which serve the exception entry function:
 *
 * 1) Synchronous exception (Offset 0x000, 0x200, 0x400, 0x600)
 * 2) IRQ                   (Offset 0x080, 0x280, 0x480, 0x680)
 * 3) FIQ                   (Offset 0x100, 0x300, 0x500, 0x700)
 * 4) SError                (Offset 0x180, 0x380, 0x580, 0x780)
 *
 * Therefore, each entry function can use up to 0x80 bytes for instructions.
 *
 * see D1.10.2 Exception vectors of Arm Architecture Reference Manual
 * Armv8, for Armv8-A architecture profile
 */

.global current_el_sp_el0_fiq_entry
.global current_el_sp_el0_irq_entry
.global current_el_sp_el0_serror_entry
.global current_el_sp_el0_sync_entry
.global current_el_sp_elx_fiq_entry
.global current_el_sp_elx_irq_entry
.global current_el_sp_elx_serror_entry
.global current_el_sp_elx_sync_entry
.global lower_el_aarch32_fiq_entry
.global lower_el_aarch32_irq_entry
.global lower_el_aarch32_serror_entry
.global lower_el_aarch32_sync_entry
.global lower_el_aarch64_fiq_entry
.global lower_el_aarch64_irq_entry
.global lower_el_aarch64_serror_entry
.global lower_el_aarch64_sync_entry

.global restore_current_el_sp_el0_fiq_entry
.global restore_current_el_sp_el0_irq_entry
.global restore_current_el_sp_el0_serror_entry
.global restore_current_el_sp_el0_sync_entry
.global restore_current_el_sp_elx_fiq_entry
.global restore_current_el_sp_elx_irq_entry
.global restore_current_el_sp_elx_serror_entry
.global restore_current_el_sp_elx_sync_entry
.global restore_lower_el_aarch32_fiq_entry
.global restore_lower_el_aarch32_irq_entry
.global restore_lower_el_aarch32_serror_entry
.global restore_lower_el_aarch32_sync_entry
.global restore_lower_el_aarch64_fiq_entry
.global restore_lower_el_aarch64_irq_entry
.global restore_lower_el_aarch64_serror_entry
.global restore_lower_el_aarch64_sync_entry

/**
 * @var EXCEPTION_FRAME_SIZE
 * @brief Size of stack frame of exception handler
 * @details
 * - 31 Registers (x0 - x30): 8 * 31 = 248
 * - SP_EL0: 8
 * - ELR_EL1: 8
 * - SPSR_EL1:8
 * - Total: 272
 */
.set EXCEPTION_FRAME_SIZE, 272

/**
 * @fn exception_handler
 * @brief Save register state and branch to handler funtion
 * @param name Symbol used for addressing this function
 * @param fn   Actual handler
 */
.macro	exception_handler name fn
\name:
	// Prepare stack frame
	sub	sp, sp, EXCEPTION_FRAME_SIZE

	// Save general purpose register x0 - x29
	stp	x0, x1, [sp, 16 * 0]
	stp	x2, x3, [sp, 16 * 1]
	stp	x4, x5, [sp, 16 * 2]
	stp	x6, x7, [sp, 16 * 3]
	stp	x8, x9, [sp, 16 * 4]
	stp	x10, x11, [sp, 16 * 5]
	stp	x12, x13, [sp, 16 * 6]
	stp	x14, x15, [sp, 16 * 7]
	stp	x16, x17, [sp, 16 * 8]
	stp	x18, x19, [sp, 16 * 9]
	stp	x20, x21, [sp, 16 * 10]
	stp	x22, x23, [sp, 16 * 11]
	stp	x24, x25, [sp, 16 * 12]
	stp	x26, x27, [sp, 16 * 13]
	stp	x28, x29, [sp, 16 * 14]

	// Save SP_EL0 and x30
	mrs x10, SP_EL0
	stp x30, x10, [sp, 16 * 15]

	// Save ELR_EL1 and SPSR_EL1
	mrs x10, ELR_EL1
	mrs x11, SPSR_EL1
	stp x10, x11, [sp, 16 * 16]

	// Pass pointer to register file to fn
	mov x0, sp

	// Branch to actual handler
	bl \fn
restore_\name:

	// Restore ELR_EL1 and SPSR_EL1
	ldp x10, x11, [sp, 16 * 16]
	msr SPSR_EL1, x11
	msr ELR_EL1, x10

	// Restore SP_EL0 and x30
	ldp x30, x10, [sp, 16 * 15]
	msr SP_EL0, x10

	// Restore general purpose register x0 - x29
	ldp	x0, x1, [sp, 16 * 0]
	ldp	x2, x3, [sp, 16 * 1]
	ldp	x4, x5, [sp, 16 * 2]
	ldp	x6, x7, [sp, 16 * 3]
	ldp	x8, x9, [sp, 16 * 4]
	ldp	x10, x11, [sp, 16 * 5]
	ldp	x12, x13, [sp, 16 * 6]
	ldp	x14, x15, [sp, 16 * 7]
	ldp	x16, x17, [sp, 16 * 8]
	ldp	x18, x19, [sp, 16 * 9]
	ldp	x20, x21, [sp, 16 * 10]
	ldp	x22, x23, [sp, 16 * 11]
	ldp	x24, x25, [sp, 16 * 12]
	ldp	x26, x27, [sp, 16 * 13]
	ldp	x28, x29, [sp, 16 * 14]

	// Clear stack frame and perform exception return
	add	sp, sp, EXCEPTION_FRAME_SIZE
	eret
.endm

/**
 * @fn exception_handler_wrapper
 * @brief Entry within exception handler which branches to exception_handler
 * @param name Name of actual exception handler
 */
.macro	exception_handler_wrapper name
.align 7
	b \name
.endm

/*
 * .text section
 */
.section .text

// Actual handler
exception_handler current_el_sp_el0_sync_entry   current_el_sp_el0_sync
exception_handler current_el_sp_el0_irq_entry    current_el_sp_el0_irq
exception_handler current_el_sp_el0_fiq_entry    current_el_sp_el0_fiq
exception_handler current_el_sp_el0_serror_entry current_el_sp_el0_serror

exception_handler current_el_sp_elx_sync_entry   current_el_sp_elx_sync
exception_handler current_el_sp_elx_irq_entry    current_el_sp_elx_irq
exception_handler current_el_sp_elx_fiq_entry    current_el_sp_elx_fiq
exception_handler current_el_sp_elx_serror_entry current_el_sp_elx_serror

exception_handler lower_el_aarch64_sync_entry   lower_el_aarch64_sync
exception_handler lower_el_aarch64_irq_entry    lower_el_aarch64_irq
exception_handler lower_el_aarch64_fiq_entry    lower_el_aarch64_fiq
exception_handler lower_el_aarch64_serror_entry lower_el_aarch64_serror

exception_handler lower_el_aarch32_sync_entry   lower_el_aarch32_sync
exception_handler lower_el_aarch32_irq_entry    lower_el_aarch32_irq
exception_handler lower_el_aarch32_fiq_entry    lower_el_aarch32_fiq
exception_handler lower_el_aarch32_serror_entry lower_el_aarch32_serror


/**
 * @var exception_vector
 * @brief Exception vector
 */
.global exception_vector
.align 11
exception_vector:
exception_handler_wrapper current_el_sp_el0_sync_entry
exception_handler_wrapper current_el_sp_el0_irq_entry
exception_handler_wrapper current_el_sp_el0_fiq_entry
exception_handler_wrapper current_el_sp_el0_serror_entry

exception_handler_wrapper current_el_sp_elx_sync_entry
exception_handler_wrapper current_el_sp_elx_irq_entry
exception_handler_wrapper current_el_sp_elx_fiq_entry
exception_handler_wrapper current_el_sp_elx_serror_entry

exception_handler_wrapper lower_el_aarch64_sync_entry
exception_handler_wrapper lower_el_aarch64_irq_entry
exception_handler_wrapper lower_el_aarch64_fiq_entry
exception_handler_wrapper lower_el_aarch64_serror_entry

exception_handler_wrapper lower_el_aarch32_sync_entry
exception_handler_wrapper lower_el_aarch32_irq_entry
exception_handler_wrapper lower_el_aarch32_fiq_entry
exception_handler_wrapper lower_el_aarch32_serror_entry
