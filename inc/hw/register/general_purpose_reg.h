#ifndef _INC_HW_REGISTER_GENERAL_PURPOSE_REGISTERS_H_
#define _INC_HW_REGISTER_GENERAL_PURPOSE_REGISTERS_H_

#include <cstdint.h>

/**
 * @file hw/register/general_purpose_reg.h
 * @brief Access to the current register value of x0 - x30
 */


#define x0(result)  asm("mov %0, x0"  : "=r"(result));

#define x1(result)  asm("mov %0, x1"  : "=r"(result));

#define x2(result)  asm("mov %0, x2"  : "=r"(result));

#define x3(result)  asm("mov %0, x3"  : "=r"(result));

#define x4(result)  asm("mov %0, x4"  : "=r"(result));

#define x5(result)  asm("mov %0, x5"  : "=r"(result));

#define x6(result)  asm("mov %0, x6"  : "=r"(result));

#define x7(result)  asm("mov %0, x7"  : "=r"(result));

#define x8(result)  asm("mov %0, x8"  : "=r"(result));

#define x9(result)  asm("mov %0, x9"  : "=r"(result));

#define x10(result) asm("mov %0, x10" : "=r"(result));

#define x11(result) asm("mov %0, x11" : "=r"(result));

#define x12(result) asm("mov %0, x12" : "=r"(result));

#define x13(result) asm("mov %0, x13" : "=r"(result));

#define x14(result) asm("mov %0, x14" : "=r"(result));

#define x15(result) asm("mov %0, x15" : "=r"(result));

#define x16(result) asm("mov %0, x16" : "=r"(result));

#define x17(result) asm("mov %0, x17" : "=r"(result));

#define x18(result) asm("mov %0, x18" : "=r"(result));

#define x19(result) asm("mov %0, x19" : "=r"(result));

#define x20(result) asm("mov %0, x20" : "=r"(result));

#define x21(result) asm("mov %0, x21" : "=r"(result));

#define x22(result) asm("mov %0, x22" : "=r"(result));

#define x23(result) asm("mov %0, x23" : "=r"(result));

#define x24(result) asm("mov %0, x24" : "=r"(result));

#define x25(result) asm("mov %0, x25" : "=r"(result));

#define x26(result) asm("mov %0, x26" : "=r"(result));

#define x27(result) asm("mov %0, x27" : "=r"(result));

#define x28(result) asm("mov %0, x28" : "=r"(result));

#define x29(result) asm("mov %0, x29" : "=r"(result));

#define x30(result) asm("mov %0, x30" : "=r"(result));

#endif /* ifndef _INC_HW_REGISTER_GENERAL_PURPOSE_REGISTERS_H_ */
