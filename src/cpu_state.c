#include "cpu_state.h"
#include "cpu_macros.h"
#include "cpu_mem.h"
#include <stdlib.h>

/// @brief Lookup table where opcode is assigned to CPU instruction with
/// specific per-cycle steps
static const CpuInstruction INSTRUCTION_TABLE[256] = {
    [CPU_OP_LDA_IMMEDIATE] = {.first_step = cpu_addr_immediate,
                              .alu_op = cpu_alu_lda},
    [CPU_OP_LDX_IMMEDIATE] = {.first_step = cpu_addr_immediate,
                              .alu_op = cpu_alu_ldx},
    [CPU_OP_LDY_IMMEDIATE] = {.first_step = cpu_addr_immediate,
                              .alu_op = cpu_alu_ldy}};

/// @brief Set/unset the zero flag in the status register based on input
/// @param cpu The emulated 6502 CPU
/// @param data The data to test
static void cpu_status_reg_update_zero(Cpu *cpu, CpuByte data) {
    cpu->sr =
        (data == 0) ? ((cpu->sr) | CPU_FLAG_Z) : ((cpu->sr) & ~CPU_FLAG_Z);
}

/// @brief Set/unset the negative flag in the status register based on input
/// @param cpu The emulated 6502 CPU
/// @param data The data to test
static void cpu_status_reg_update_negative(Cpu *cpu, CpuByte data) {
    cpu->sr =
        (data & 0x80) ? ((cpu->sr) | CPU_FLAG_N) : ((cpu->sr) & ~CPU_FLAG_N);
}

int cpu_inst_implemented(CpuByte opcode) {
    return INSTRUCTION_TABLE[opcode].first_step == NULL;
}

void cpu_tick(Cpu *cpu) { cpu->cur_step(cpu); }

void cpu_fetch_opcode(Cpu *cpu) {
    // Read opcode from PC
    cpu->opcode = cpu_mem_read(cpu, cpu->pc++);
    // Get instruction from opcode
    CpuInstruction instruction = INSTRUCTION_TABLE[cpu->opcode];

    if (instruction.first_step == NULL) {
        return; // NOP - invalid instruction pipeline
    }

    // Save operation being carried out until address resolved
    cpu->pending_alu = instruction.alu_op;
    // Start pipeline
    cpu->cur_step = instruction.first_step;
}

void cpu_addr_immediate(Cpu *cpu) {
    // Read byte after opcode
    cpu_mem_read(cpu, cpu->pc++);

    // Run pending ALU operation using fetched data
    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Operation complete, fetch new opcode
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_alu_lda(Cpu *cpu) {
    // Load data bus content into accumulator
    cpu->a = cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->a);
    cpu_status_reg_update_zero(cpu, cpu->a);
}

void cpu_alu_ldx(Cpu *cpu) {
    // Load data bus content into accumulator
    cpu->x = cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->x);
    cpu_status_reg_update_zero(cpu, cpu->x);
}

void cpu_alu_ldy(Cpu *cpu) {
    // Load data bus content into accumulator
    cpu->y = cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->y);
    cpu_status_reg_update_zero(cpu, cpu->y);
}