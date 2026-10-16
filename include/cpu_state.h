#ifndef CPU_STATE_H
#define CPU_STATE_H

#include "cpu_macros.h"
#include "cpu_types.h"

/// @brief Representation of 6502 CPU register state
typedef struct Cpu {
    // Internal registers
    CpuByte a, x, y, sr, sp;
    CpuWord pc;

    // Internal latches
    CpuByte opcode;
    CpuWord addr_bus;
    CpuByte data_bus;
    CpuByte is_read;

    // State machine pointer
    void (*cur_step)(struct Cpu *);
    void (*pending_alu)(struct Cpu *);

    // For testing - is 1 if opcode lookup fails
    CpuByte not_impl;

    // CPU Memory
    CpuByte mem[CPU_MEM_MAX_SIZE];
} Cpu;

/// @brief Function that executes exactly one clock cycle of work
typedef void (*CpuStep)(Cpu *);
/// @brief Function that executes exactly combinational logic of ALU
typedef void (*AluOperation)(Cpu *);

/// @brief Instruction that can be run on the CPU possibly spanning multiple
/// clock cycles
typedef struct {
    CpuStep first_step;
    AluOperation alu_op;
} CpuInstruction;

/// @brief Performs one clock cycle on the CPU
/// @param cpu The emulated emulated 6502 CPU
void cpu_tick(Cpu *cpu);

/// @brief Reads the current opcode in the program counter before incrementing
/// it. Then performs decode logic to get instruction steps
/// @param cpu The emulated 6502 CPU
void cpu_fetch_opcode(Cpu *cpu);

/// @brief Handle immediate addressing and run pending ALU
/// @param cpu The emulated 6502 CPU
void cpu_addr_immediate(Cpu *cpu);

/// @brief LDA Instruction ALU logic: Load accumulator with memory in data bus
/// @param cpu The emulated 6502 CPU
void cpu_alu_lda(Cpu *cpu);

#endif // CPU_STATE_H