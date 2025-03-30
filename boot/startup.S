.global _start
.global _endless_loop
.global __CPU_ID
.global __CPU_STACK

.set PAGESIZE,  4096
.set STACKALGN, 16
.set STACKSIZE, (4 * 4096)

/*
 * Startup Code
 * - Register x0 contains physical address of device tree blob (dtb)
 */

/*
 * .boot section
 */
.section .boot

.func
_start:

// Get CPU ID in x1
mrs x1, MPIDR_EL1
and x1, x1, 0xFF

// Wait till __CPU_ID increment and match own CPU ID
.waiting_cpu_loop:
	ldr  x2, __CPU_ID
	cmp x1, x2
	bne .waiting_cpu_loop

// Perform exception return if CurrentEL != EL1 (x1 should be 0b01)
// See 5.5 Changing Exception levels of Bare-metal Boot Code for ARMv8-A Processors
	mrs x1, CurrentEL
	and x1, x1, 0xF
	lsr x1, x1, 0x2
	cmp x1, 0b01 // CurrentEL == EL1
	beq .setup_stack
	cmp x1, 0b10 // CurrentEL == EL2
	beq .leave_el2

// Perform exception return: EL3 -> EL2
.leave_el3:
	msr SCTLR_EL2, xzr
	msr HCR_EL2, xzr
	mrs x1, SCR_EL3
	orr x1, x1, (1 << 10) // EL2 Aarch64
	orr x1, x1, (1 << 0)  // Non-Secure
	msr SCR_EL3, x1
	mov x1, 0b01001       // DAIF=0000
	msr SPSR_EL3, x1
	adr x1, .leave_el2
	msr ELR_EL3, x1       // EL2 code
	eret

// Perform exception return: EL2 -> EL1
.leave_el2:
	msr SCTLR_EL1, xzr
	mrs x1, HCR_EL2
	orr x1, x1, (1 << 31) // EL1 Aarch64
	msr HCR_EL2, x1
	mov x1, 0b00101       // DAIF=0000
	msr SPSR_EL2, x1
	adr x1, .setup_stack  // EL1 code
	msr ELR_EL2, x1
	eret

// Setup stack for EL1
.setup_stack:
	mrs x1, CUrrentEL
	ldr x1, __CPU_STACK
	mov sp, x1

	// Reset SCTLR
	mov x1, 0
	msr SCTLR_EL1, x1

	// Get CPU ID in x1
	mrs x1, MPIDR_EL1
	and x1, x1, 0xFF

	// Jump to C++ Code (x0: fdt, x1: cpuID)
	bl kernelMainWrapper

.endfunc

// Endless loop should *never* be reached
.func
_endless_loop:
	wfe
	b _endless_loop
.endfunc

/*
 * .data section
 */
.section .data

// Stack of the boot CPU
.align 16
boot_cpu_stack:
	.fill (4 * PAGESIZE)

// Highest active CPU ID
__CPU_ID:
	.quad 0

// Current stack of the highest active CPU
__CPU_STACK:
	.quad boot_cpu_stack + STACKSIZE - STACKALGN

