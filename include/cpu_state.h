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

/// @brief Checks if a given opcode has actually been implemented by the CPU
/// @param opcode The 6502 opcode to check
/// @return 0 if implemented, 1 otherwise
int cpu_inst_implemented(CpuByte opcode);

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

/// @brief Handle implied addressing and run pending ALU
/// @param cpu The emulated 6502 CPU
void cpu_addr_implied(Cpu *cpu);

/// @brief LDA Instruction ALU logic: Load accumulator with memory in data bus
/// @param cpu The emulated 6502 CPU
void cpu_alu_lda(Cpu *cpu);

/// @brief LDX Instruction ALU logic: Load X with memory in data bus
/// @param cpu The emulated 6502 CPU
void cpu_alu_ldx(Cpu *cpu);

/// @brief LDY Instruction ALU logic: Load Y with memory in data bus
/// @param cpu The emulated 6502 CPU
void cpu_alu_ldy(Cpu *cpu);

/// @brief AND Instruction ALU logic: AND data bus with accumulator and store
/// back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_and(Cpu *cpu);

/// @brief ORA Instruction ALU logic: OR data bus with accumulator and store
/// back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_ora(Cpu *cpu);

/// @brief EOR Instruction ALU logic: XOR data bus with accumulator and store
/// back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_eor(Cpu *cpu);

/// @brief ADC Instruction ALU logic: add data bus with accumulator with carry
/// and store back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_adc(Cpu *cpu);

/// @brief CMP Instruction ALU logic: CMP (subtract) data bus with accumulator
/// and update flags without storing result back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_cmp(Cpu *cpu);

/// @brief CPY Instruction ALU logic: CMP (subtract) data bus with Y
/// and update flags without storing result back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_cpy(Cpu *cpu);

/// @brief CPX Instruction ALU logic: CMP (subtract) data bus with X
/// and update flags without storing result back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_cpx(Cpu *cpu);

/// @brief SBC Instruction ALU logic: subtract data bus from accumulator with
/// carry and store back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_sbc(Cpu *cpu);

/// @brief SED Instruction ALU logic: Set the decimal flag in the status
/// register
/// @param cpu The emulated 6502 CPU
void cpu_alu_sed(Cpu *cpu);

/// @brief NOP Instruction ALU logic: Do nothing
/// @param cpu The emulated 6502 CPU
void cpu_alu_nop(Cpu *cpu);

/// @brief CLC Instruction ALU logic: Unset carry flag in status register
/// @param cpu The emulated 6502 CPU
void cpu_alu_clc(Cpu *cpu);

/// @brief SEC Instruction ALU logic: Set carry flag in status register
/// @param cpu The emulated 6502 CPU
void cpu_alu_sec(Cpu *cpu);

/// @brief DEY Instruction ALU logic: Decrement Y by 1
/// @param cpu The emulated 6502 CPU
void cpu_alu_dey(Cpu *cpu);

/// @brief DEX Instruction ALU logic: Decrement X by 1
/// @param cpu The emulated 6502 CPU
void cpu_alu_dex(Cpu *cpu);

/// @brief TXA Instruction ALU logic: Copy X into A
/// @param cpu The emulated 6502 CPU
void cpu_alu_txa(Cpu *cpu);

/// @brief TYA Instruction ALU logic: Copy Y into A
/// @param cpu The emulated 6502 CPU
void cpu_alu_tya(Cpu *cpu);

/// @brief TAX Instruction ALU logic: Copy A into X
/// @param cpu The emulated 6502 CPU
void cpu_alu_tax(Cpu *cpu);

/// @brief TAY Instruction ALU logic: Copy A into Y
/// @param cpu The emulated 6502 CPU
void cpu_alu_tay(Cpu *cpu);

/// @brief CLV Instruction ALU logic: Unset the overflow status register flag
/// @param cpu The emulated 6502 CPU
void cpu_alu_clv(Cpu *cpu);

#endif // CPU_STATE_H