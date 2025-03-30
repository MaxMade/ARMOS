.global __context_switch

/**
 * @file kernel/thread/context_switch.S
 * @brief Low-Level context switch
 */

.section .text

/* void __context_switch(SavedContext* old, SavedContext* next)
 * Perform context switch for following struct definition:
 * struct SavedContext { 
 *		uint64_t x19; // Offset 0x00
 * 		uint64_t x20; // Offset 0x08
 * 		uint64_t x21; // Offset 0x10
 * 		uint64_t x22; // Offset 0x18
 * 		uint64_t x23; // Offset 0x20
 * 		uint64_t x24; // Offset 0x28
 * 		uint64_t x25; // Offset 0x30
 * 		uint64_t x26; // Offset 0x38
 * 		uint64_t x27; // Offset 0x40
 * 		uint64_t x28; // Offset 0x48
 * 		uint64_t x29; // Offset 0x50
 * 		uint64_t x30; // Offset 0x58
 * 		uint64_t sp;  // Offset 0x60
 * }
 */
__context_switch:
	stp x19, x20, [x0, 0x00]
	stp x21, x22, [x0, 0x10]
	stp x23, x24, [x0, 0x20]
	stp x25, x26, [x0, 0x30]
	stp x27, x28, [x0, 0x40]
	stp x29, x30, [x0, 0x50]

	mov x8, sp
	str x8, [x0, 0x60]

	ldr x8, [x1, 0x60]
	mov sp, x8

	ldp x29, x30, [x1, 0x50]
	ldp x27, x28, [x1, 0x40]
	ldp x25, x26, [x1, 0x30]
	ldp x23, x24, [x1, 0x20]
	ldp x21, x22, [x1, 0x10]
	ldp x19, x20, [x1, 0x00]

	ret
