#ifndef CPU_MACROS_H
#define CPU_MACROS_H

// Max/min sizes
#define CPU_MEM_MAX_SIZE (256 * 256)

// Important memory locations
#define CPU_MEM_RESET_VEC_ADDR_H 0xFFFC
#define CPU_MEM_RESET_VEC_ADDR_L 0xFFFD
#define CPU_MEM_STACK_START 0x0100
#define CPU_MEM_STACK_END 0x1FF

// Flags
#define CPU_IS_READ 1
#define CPU_IS_WRITE 0

// Op codes

// Immediate addressing
#define CPU_OP_ORA_IMMEDIATE 0x09
#define CPU_OP_AND_IMMEDIATE 0x29
#define CPU_OP_EOR_IMMEDIATE 0x49
#define CPU_OP_ADC_IMMEDIATE 0x69
#define CPU_OP_LDY_IMMEDIATE 0xA0
#define CPU_OP_LDX_IMMEDIATE 0xA2
#define CPU_OP_LDA_IMMEDIATE 0xA9
#define CPU_OP_CPY_IMMEDIATE 0xC0
#define CPU_OP_CMP_IMMEDIATE 0xC9
#define CPU_OP_CPX_IMMEDIATE 0xE0
#define CPU_OP_SBC_IMMEDIATE 0xE9

// Implied addressing
#define CPU_OP_CLC_IMPLIED 0x18
#define CPU_OP_SEC_IMPLIED 0x38
#define CPU_OP_DEY_IMPLIED 0x88
#define CPU_OP_NOP_IMPLIED 0xEA
#define CPU_OP_SED_IMPLIED 0xF8

// Status register masks
#define CPU_FLAG_N (1 << 7)
#define CPU_FLAG_V (1 << 6)
#define CPU_FLAG_B (1 << 4)
#define CPU_FLAG_D (1 << 3)
#define CPU_FLAG_I (1 << 2)
#define CPU_FLAG_Z (1 << 1)
#define CPU_FLAG_C (1 << 0)

#endif // CPU_MACROS_H