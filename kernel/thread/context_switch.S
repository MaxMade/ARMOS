.global __context_switch
.global __context_kickoff

.extern __unlock_for_kickoff
.extern __unlock_spinlock
.extern restore_current_el_sp_el0_sync_entry

/**
 * @file kernel/thread/context_switch.S
 * @brief Low-Level context switch
 */

.section .text

/* void __context_switch(SavedContext* old, SavedContext* next, lock::spinlock* lock)
 * Perform context switch for following struct definition:
 * struct SavedContext { 
 *		uint64_t x18; // Offset 0x00
 *		uint64_t x19; // Offset 0x08
 * 		uint64_t x20; // Offset 0x10
 * 		uint64_t x21; // Offset 0x18
 * 		uint64_t x22; // Offset 0x20
 * 		uint64_t x23; // Offset 0x28
 * 		uint64_t x24; // Offset 0x30
 * 		uint64_t x25; // Offset 0x38
 * 		uint64_t x26; // Offset 0x40
 * 		uint64_t x27; // Offset 0x48
 * 		uint64_t x28; // Offset 0x50
 * 		uint64_t x29; // Offset 0x58
 * 		uint64_t x30; // Offset 0x60
 * 		uint64_t sp;  // Offset 0x68
 * }
 */
__context_switch:
	stp x18, x19, [x0, 0x00]
	stp x20, x21, [x0, 0x10]
	stp x22, x23, [x0, 0x20]
	stp x24, x25, [x0, 0x30]
	stp x26, x27, [x0, 0x40]
	stp x28, x29, [x0, 0x50]

	mov x8, sp
	stp x30, x8,  [x0, 0x60]

	ldp x30, x8,  [x1, 0x60]
	mov sp, x8

	ldp x28, x29, [x1, 0x50]
	ldp x26, x27, [x1, 0x40]
	ldp x24, x25, [x1, 0x30]
	ldp x22, x23, [x1, 0x20]
	ldp x20, x21, [x1, 0x10]
	ldp x18, x19, [x1, 0x00]

	cmp x2, 0
	b.eq .unlocked
	mov x0, x2
	bl __unlock_spinlock

.unlocked:
	ret

/* void __context_kickoff
 * Perform __unlock_for_kickoff and update stack pointer according to x19.
 */
__context_kickoff:
	bl __unlock_for_kickoff
	mov sp, x19
	b restore_current_el_sp_el0_sync_entry
