#include "cpu_addr.h"
#include "cpu_mem.h"
#include "cpu_state.h"
#include "cpu_types.h"
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

void cpu_addr_absolute_low(Cpu* cpu) {
    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_absolute_high;
}

void cpu_addr_absolute_high(Cpu* cpu) {
    // Read the high byte
    cpu->addr_operand = ((CpuWord)(cpu_mem_read(cpu, cpu->pc++)) << 8) | cpu->addr_operand;
    cpu->cur_step = cpu_addr_aboslute;
}

void cpu_addr_aboslute(Cpu* cpu) {
    // Read data from opcode address
    cpu_mem_read(cpu, cpu->addr_operand);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Operation complete, fetch new opcode
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_absolute_rmw_low(Cpu* cpu) {
    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_absolute_rmw_high;
}

void cpu_addr_absolute_rmw_high(Cpu* cpu) {
    // Read the high byte
    cpu->addr_operand = ((CpuWord)(cpu_mem_read(cpu, cpu->pc++)) << 8) | cpu->addr_operand;
    cpu->cur_step = cpu_addr_absolute_rmw_read;
}

void cpu_addr_absolute_rmw_read(Cpu* cpu) {
    // Read data from opcode address
    cpu_mem_read(cpu, cpu->addr_operand);

    cpu->cur_step = cpu_addr_absolute_rmw_write;
}

void cpu_addr_absolute_rmw_write(Cpu* cpu) {
    // Dummy write before operation
    cpu_mem_write(cpu, cpu->addr_bus, cpu->data_bus);

    cpu->cur_step = cpu_addr_absolute_rmw_write2;
}

void cpu_addr_absolute_rmw_write2(Cpu* cpu) {
    // Do operation
    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Back to fetching opcodes
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_zpg_low(Cpu* cpu) {
    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_zpg;
}

void cpu_addr_zpg(Cpu* cpu) {
    // Read data from opcode address
    cpu_mem_read(cpu, cpu->addr_operand);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Operation complete, fetch new opcode
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_zpg_rmw_low(Cpu* cpu) {
    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_zpg_rmw_read;
}

void cpu_addr_zpg_rmw_read(Cpu* cpu) {
    // Read data from opcode address
    cpu_mem_read(cpu, cpu->addr_operand);

    cpu->cur_step = cpu_addr_zpg_rmw_write;
}

void cpu_addr_zpg_rmw_write(Cpu* cpu) {
    // Dummy write before operation
    cpu_mem_write(cpu, cpu->addr_bus, cpu->data_bus);

    cpu->cur_step = cpu_addr_zpg_rmw_write2;
}

void cpu_addr_zpg_rmw_write2(Cpu* cpu) {
    // Do operation
    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Back to fetching opcodes
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_abs_x_low(Cpu* cpu) {
    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_abs_x_high;
}

void cpu_addr_abs_x_high(Cpu* cpu) {
    // Read the high byte
    cpu_mem_read(cpu, cpu->pc++);

    // Add X to the low byte
    cpu->addr_operand += cpu->x;

    cpu->cur_step = cpu_addr_abs_x_read1;
}

void cpu_addr_abs_x_read1(Cpu* cpu) {
    CpuByte high = cpu->data_bus;
    CpuWord low = cpu->addr_operand;

    // Perform potentially invalid read
    // High byte is still on the address bus
    cpu_mem_read(cpu, ((CpuWord)(high << 8) | (low & 0xFF)));

    // Read was valid without fix
    if (low < 0x100) {
        if (cpu->pending_alu) {
            cpu->pending_alu(cpu);
        }
        cpu->cur_step = cpu_fetch_opcode;
        return;
    }

    // Page boundary was crossed, second read performed must correct
    cpu->addr_operand = ((CpuWord)(high + 1) << 8) | (low & 0xFF);

    cpu->cur_step = cpu_addr_abs_x_read2;
}

void cpu_addr_abs_x_read2(Cpu* cpu) {
    cpu_mem_read(cpu, cpu->addr_operand);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    cpu->cur_step = cpu_fetch_opcode;
}
