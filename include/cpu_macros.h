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
#define CPU_OP_LDA_IMMEDIATE 0xA9
#define CPU_OP_LDX_IMMEDIATE 0xA2
#define CPU_OP_LDY_IMMEDIATE 0xA0
#define CPU_OP_AND_IMMEDIATE 0x29

// Status register masks
#define CPU_FLAG_N (1 << 7)
#define CPU_FLAG_V (1 << 6)
#define CPU_FLAG_B (1 << 4)
#define CPU_FLAG_D (1 << 3)
#define CPU_FLAG_I (1 << 2)
#define CPU_FLAG_Z (1 << 1)
#define CPU_FLAG_C (1 << 0)

#endif // CPU_MACROS_H