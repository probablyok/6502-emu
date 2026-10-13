#ifndef CPU_STATE_H
#define CPU_STATE_H

#include <cpu_types.h>

/// @brief Representation of 6502 CPU register state
typedef struct {
    CpuWord pc;
    CpuByte a;
    CpuByte x;
    CpuByte y;
    CpuByte sr;
    CpuByte sp;
} Cpu;

/// @brief All 6502 CPU Opcodes
typedef enum {
    OP_ADC,
    OP_AND,
    OP_ASL,
    OP_BCC,
    OP_BCS,
    OP_BEQ,
    OP_BIT,
    OP_BMI,
    OP_BNE,
    OP_BPL,
    OP_BRK,
    OP_BVC,
    OP_BVS,
    OP_CLC,
    OP_CLD,
    OP_CLI,
    OP_CLV,
    OP_CMP,
    OP_CPX,
    OP_CPY,
    OP_DEC,
    OP_DEX,
    OP_DEY,
    OP_EOR,
    OP_INC,
    OP_INX,
    OP_INY,
    OP_JMP,
    OP_JSR,
    OP_LDA,
    OP_LDX,
    OP_LDY,
    OP_LSR,
    OP_NOP,
    OP_ORA,
    OP_PHA,
    OP_PHP,
    OP_PLA,
    OP_PLP,
    OP_ROL,
    OP_ROR,
    OP_RTI,
    OP_RTS,
    OP_SBC,
    OP_SEC,
    OP_SED,
    OP_SEI,
    OP_STA,
    OP_STX,
    OP_STY,
    OP_TAX,
    OP_TAY,
    OP_TSX,
    OP_TXA,
    OP_TXS,
    OP_TYA,
    OP_ILLEGAL
} Opcode;

/// @brief All addressing modes for 6502 CPU opcodes
typedef enum {
    ADDR_ACCUMULATOR,
    ADDR_ABS,
    ADDR_ABS_X,
    ADDR_ABS_Y,
    ADDR_IMMEDIATE,
    ADDR_IMPLIED,
    ADDR_INDIRECT,
    ADDR_IDX_INDIRECT,
    ADDR_INDIRECT_IDY,
    ADDR_RELATIVE,
    ADDR_ZEROPAGE,
    ADDR_ZEROPAGE_IDX,
    ADDR_ZEROPAGE_IDY,
} AddrMode;

/// @brief Zeros all CPU registers
/// @param cpu The emulated 6502 CPU
void cpu_state_zero(Cpu *cpu);

/// @brief Creates 256x256 array containing bytes of memory
/// @return Memory array
CpuByte *cpu_create_mem(void);

#endif // CPU_STATE_H