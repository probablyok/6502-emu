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
    void (*cur_step)(struct Cpu*);
    void (*pending_alu)(struct Cpu*);

    // Keep track of multi-cycle address reads
    CpuWord addr_operand;

    // Indirect addressing write location before setting PC
    CpuWord ind_latch;

    // Flag for indexed addressing. 0 for X reg, 1 for Y reg
    CpuByte idx_reg;

    // CPU Memory
    CpuByte mem[CPU_MEM_MAX_SIZE];
} Cpu;

/// @brief Function that executes exactly one clock cycle of work
typedef void (*CpuStep)(Cpu*);
/// @brief Function that executes exactly combinational logic of ALU
typedef void (*AluOperation)(Cpu*);

/// @brief Instruction that can be run on the CPU possibly spanning multiple
/// clock cycles
typedef struct {
    CpuStep first_step;
    AluOperation alu_op;
} CpuInstruction;

/// @brief Lookup table where opcode is assigned to CPU instruction with
/// specific per-cycle steps
extern const CpuInstruction INSTRUCTION_TABLE[256];

/// @brief Checks if a given opcode has actually been implemented by the CPU
/// @param opcode The 6502 opcode to check
/// @return 0 if implemented, 1 otherwise
int cpu_inst_implemented(CpuByte opcode);

/// @brief Performs one clock cycle on the CPU
/// @param cpu The emulated emulated 6502 CPU
void cpu_tick(Cpu* cpu);

/// @brief Set/unset the zero flag in the status register based on input
/// @param cpu The emulated 6502 CPU
/// @param data The data to test
void cpu_status_reg_update_zero(Cpu* cpu, CpuByte data);

/// @brief Set/unset the negative flag in the status register based on input
/// @param cpu The emulated 6502 CPU
/// @param data The data to test
void cpu_status_reg_update_negative(Cpu* cpu, CpuByte data);

/// @brief Set/unset the carry flag in the status register based on true or
/// false condition provided
/// @param cpu The emulated 6502 CPU
/// @param condition The condition used to set the carry flag
void cpu_status_reg_update_carry(Cpu* cpu, int condition);

/// @brief Set/unset the overflow flag in the status register based on true or
/// false condition provided
/// @param cpu The emulated 6502 CPU
/// @param condition The condition used to set the overflow flag
void cpu_status_reg_update_overflow(Cpu* cpu, int condition);

/// @brief Set/unset the BCD flag in the status register based on input
/// @param cpu The emulated 6502 CPU
/// @param condition The data to set
void cpu_status_reg_update_decimal(Cpu* cpu, int data);

/// @brief Set/unset the interrupt disable flag in the status register based on
/// input
/// @param cpu The emulated 6502 CPU
/// @param condition The data to set
void cpu_status_reg_update_interrupt_disable(Cpu* cpu, int data);

#endif // CPU_STATE_H
