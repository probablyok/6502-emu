#include "cpu_addr.h"
#include "cpu_mem.h"
#include "cpu_state.h"
#include <stdlib.h>

void cpu_fetch_opcode(Cpu* cpu) {
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

void cpu_addr_immediate(Cpu* cpu) {
    // Read byte after opcode
    cpu_mem_read(cpu, cpu->pc++);

    // Run pending ALU operation using fetched data
    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Operation complete, fetch new opcode
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_implied(Cpu* cpu) {
    // Dummy read byte after opcode
    cpu_mem_read(cpu, cpu->pc);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Operation complete, fetch new opcode
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_aboslute_read(Cpu* cpu) {
    // Read data from opcode address
    cpu_mem_read(cpu, cpu->addr_operand);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Operation complete, fetch new opcode
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_absolute_read_high(Cpu* cpu) {
    // Read the high byte
    cpu->addr_operand = ((CpuWord)(cpu_mem_read(cpu, cpu->pc++)) << 8) | cpu->addr_operand;
    cpu->cur_step = cpu_addr_aboslute_read;
}

void cpu_addr_absolute_read_low(Cpu* cpu) {
    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_absolute_read_high;
}
