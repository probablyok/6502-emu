#ifndef CPU_STATE_H
#define CPU_STATE_H

#include "cpu_types.h"

/// @brief Representation of 6502 CPU register state
typedef struct Cpu {
    // Internal registers
    CpuByte a, x, y, sr, sp;
    CpuWord pc;

    // CPU Memory
    CpuByte *mem;

    // Internal latches
    CpuByte opcode;
    CpuWord addr_bus;
    CpuByte data_bus;

    // State machine pointer
    void (*cur_step)(struct Cpu *);
    void (*pending_operation)(struct Cpu *);
} Cpu;

/// @brief Function that executes exactly one clock cycle of work
typedef void (*CpuStep)(Cpu *);

/// @brief Instruction that can be run on the CPU possibly spanning multiple
/// clock cycles
typedef struct {
    CpuStep first_step;
    CpuStep operation;
} CpuInstruction;

/// @brief All addressing modes for 6502 CPU opcodes
typedef enum {
    ADDR_ACCUMULATOR,
    ADDR_ABSOLUTE,
    ADDR_ABSOLUTE_X,
    ADDR_ABSOLUTE_Y,
    ADDR_IMMEDIATE,
    ADDR_IMPLIED,
    ADDR_INDIRECT,
    ADDR_IDX_INDIRECT,
    ADDR_INDIRECT_IDY,
    ADDR_RELATIVE,
    ADDR_ZEROPAGE,
    ADDR_ZEROPAGE_IDX,
    ADDR_ZEROPAGE_IDY,
    ADDR_ILLEGAL
} AddrMode;

/// @brief Frees the 256x256 array for memory in CPU
/// @param cpu The emulated 6502 CPU
void cpu_free_mem(Cpu *cpu);

/// @brief Performs one clock cycle on the CPU
/// @param cpu The emulated emulated 6502 CPU
void cpu_tick(Cpu *cpu);

#endif // CPU_STATE_H