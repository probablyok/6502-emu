#include "cpu_addr.h"
#include "cpu_macros.h"
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

void cpu_addr_absolute_jmp_low(Cpu* cpu) {
    // Read the low byte
    cpu->ind_latch = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_absolute_jmp_high;
}

void cpu_addr_absolute_jmp_high(Cpu* cpu) {
    // Read the high byte
    cpu->ind_latch = ((CpuWord)(cpu_mem_read(cpu, cpu->pc)) << 8) | cpu->ind_latch;

    // Set PC to new address
    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_indirect_jmp_low(Cpu* cpu) {
    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_indirect_jmp_high;
}

void cpu_addr_indirect_jmp_high(Cpu* cpu) {
    // Read the high byte
    cpu->addr_operand = ((CpuWord)(cpu_mem_read(cpu, cpu->pc)) << 8) | cpu->addr_operand;
    cpu->cur_step = cpu_addr_indirect_jmp_read1;
}

void cpu_addr_indirect_jmp_read1(Cpu* cpu) {
    // Read PCL from addr_operand
    cpu->ind_latch = cpu_mem_read(cpu, cpu->addr_operand);
    cpu->cur_step = cpu_addr_indirect_jmp_read2;
}

void cpu_addr_indirect_jmp_read2(Cpu* cpu) {
    // Move across in memory in some page with wrapping
    CpuWord next =
        (cpu->addr_operand & 0xFF00) |
        (((cpu->addr_operand & 0x00FF) == 0xFF) ? 0x00 : (cpu->addr_operand & 0x00FF) + 1);

    // Read PCH
    CpuByte high = cpu_mem_read(cpu, next);
    cpu->ind_latch = ((CpuWord)high << 8) | cpu->ind_latch;

    // Set PC to new address
    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

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

void cpu_addr_abs_idx_low_x(Cpu* cpu) {
    cpu->idx_reg = 0;

    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_abs_idx_high;
}

void cpu_addr_abs_idx_low_y(Cpu* cpu) {
    cpu->idx_reg = 1;

    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_abs_idx_high;
}

void cpu_addr_abs_idx_high(Cpu* cpu) {
    // Read the high byte
    cpu_mem_read(cpu, cpu->pc++);

    // Add X/Y to the low byte
    cpu->addr_operand += cpu->idx_reg ? cpu->y : cpu->x;

    cpu->cur_step = cpu_addr_abs_idx_read1;
}

void cpu_addr_abs_idx_read1(Cpu* cpu) {
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

    cpu->cur_step = cpu_addr_abs_idx_read2;
}

void cpu_addr_abs_idx_read2(Cpu* cpu) {
    cpu_mem_read(cpu, cpu->addr_operand);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_abs_idx_write_low_x(Cpu* cpu) {
    cpu->idx_reg = 0;

    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_abs_idx_write_high;
}

void cpu_addr_abs_idx_write_low_y(Cpu* cpu) {
    cpu->idx_reg = 1;

    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_abs_idx_write_high;
}

void cpu_addr_abs_idx_write_high(Cpu* cpu) {
    // Read the high byte
    cpu_mem_read(cpu, cpu->pc++);

    // Add X/Y to the low byte
    cpu->addr_operand += cpu->idx_reg ? cpu->y : cpu->x;

    cpu->cur_step = cpu_addr_abs_idx_write_read;
}

void cpu_addr_abs_idx_write_read(Cpu* cpu) {
    CpuByte high = cpu->data_bus;
    CpuWord low = cpu->addr_operand;

    // Perform potentially invalid read
    // High byte is still on the address bus
    cpu_mem_read(cpu, ((CpuWord)(high << 8) | (low & 0xFF)));

    // Page boundary might have been crossed, second read performed must correct if necessary
    CpuByte overflow = (low >> 8) & 0x01;
    cpu->addr_operand = ((CpuWord)(high + overflow) << 8) | (low & 0xFF);

    cpu->cur_step = cpu_addr_abs_idx_write;
}

void cpu_addr_abs_idx_write(Cpu* cpu) {
    cpu_mem_read(cpu, cpu->addr_operand);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_abs_idx_rmw_low_x(Cpu* cpu) {
    cpu->idx_reg = 0;

    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_abs_idx_rmw_high;
}

void cpu_addr_abs_idx_rmw_low_y(Cpu* cpu) {
    cpu->idx_reg = 1;

    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_abs_idx_rmw_high;
}

void cpu_addr_abs_idx_rmw_high(Cpu* cpu) {
    // Read the high byte
    cpu_mem_read(cpu, cpu->pc++);

    // Add X/Y to the low byte
    cpu->addr_operand += cpu->idx_reg ? cpu->y : cpu->x;

    cpu->cur_step = cpu_addr_abs_idx_rmw_read1;
}

void cpu_addr_abs_idx_rmw_read1(Cpu* cpu) {
    CpuByte high = cpu->data_bus;
    CpuWord low = cpu->addr_operand;

    // Perform potentially invalid read
    // High byte is still on the address bus
    cpu_mem_read(cpu, ((CpuWord)(high << 8) | (low & 0xFF)));

    // Page boundary might have been crossed, second read performed must correct if necessary
    CpuByte overflow = (low >> 8) & 0x01;
    cpu->addr_operand = ((CpuWord)(high + overflow) << 8) | (low & 0xFF);

    cpu->cur_step = cpu_addr_abs_idx_rmw_read2;
}

void cpu_addr_abs_idx_rmw_read2(Cpu* cpu) {
    // Re-read from effective address
    cpu_mem_read(cpu, cpu->addr_operand);

    cpu->cur_step = cpu_addr_abs_idx_rmw_write1;
}

void cpu_addr_abs_idx_rmw_write1(Cpu* cpu) {
    // Dummy write to effective address, then do operation after
    cpu_mem_write(cpu, cpu->addr_bus, cpu->data_bus);

    cpu->cur_step = cpu_addr_abs_idx_rmw_write2;
}

void cpu_addr_abs_idx_rmw_write2(Cpu* cpu) {
    // Operated value writes back
    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_zpg_idx_low_x(Cpu* cpu) {
    cpu->idx_reg = 0;

    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_zpg_idx_read1;
}

void cpu_addr_zpg_idx_low_y(Cpu* cpu) {
    cpu->idx_reg = 1;

    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_zpg_idx_read1;
}

void cpu_addr_zpg_idx_read1(Cpu* cpu) {
    cpu_mem_read(cpu, cpu->addr_operand);
    cpu->cur_step = cpu_addr_zpg_idx_read2;
}

void cpu_addr_zpg_idx_read2(Cpu* cpu) {
    // Add index register and read effective address
    CpuByte effective_low = cpu->addr_bus + (cpu->idx_reg ? cpu->y : cpu->x);
    cpu_mem_read(cpu, effective_low);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Operation complete, fetch new opcode
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_zpg_idx_rmw_low_x(Cpu* cpu) {
    cpu->idx_reg = 0;

    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_zpg_idx_rmw_read1;
}

void cpu_addr_zpg_idx_rmw_low_y(Cpu* cpu) {
    cpu->idx_reg = 1;

    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_zpg_idx_rmw_read1;
}

void cpu_addr_zpg_idx_rmw_read1(Cpu* cpu) {
    cpu_mem_read(cpu, cpu->addr_operand);
    cpu->cur_step = cpu_addr_zpg_idx_rmw_read2;
}

void cpu_addr_zpg_idx_rmw_read2(Cpu* cpu) {
    // Add index register and read effective address
    CpuByte effective_low = cpu->addr_bus + (cpu->idx_reg ? cpu->y : cpu->x);
    cpu_mem_read(cpu, effective_low);

    cpu->cur_step = cpu_addr_zpg_idx_rmw_write1;
}

void cpu_addr_zpg_idx_rmw_write1(Cpu* cpu) {
    // Dummy write to effective address before doing ALU
    cpu_mem_write(cpu, cpu->addr_bus, cpu->data_bus);
    cpu->cur_step = cpu_addr_zpg_idx_rmw_write2;
}

void cpu_addr_zpg_idx_rmw_write2(Cpu* cpu) {
    // Do ALU instruction that also handles writing back to effective address
    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Operation complete, fetch new opcode
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_pre_idx_indirect_low(Cpu* cpu) {
    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_pre_idx_indirect_read_ptr;
}

void cpu_addr_pre_idx_indirect_read_ptr(Cpu* cpu) {
    // Dummy read on non-effective address
    cpu_mem_read(cpu, cpu->addr_operand);

    // Add X to the address (wrap in zero-page)
    cpu->addr_operand = (CpuByte)((cpu->addr_operand & 0xFF) + cpu->x);

    cpu->cur_step = cpu_addr_pre_idx_indirect_fetch_low;
}

void cpu_addr_pre_idx_indirect_fetch_low(Cpu* cpu) {
    // Read first byte in effective address
    cpu->ind_latch = cpu_mem_read(cpu, cpu->addr_operand);

    cpu->cur_step = cpu_addr_pre_idx_indirect_fetch_high;
}

void cpu_addr_pre_idx_indirect_fetch_high(Cpu* cpu) {
    // Read second byte in effective address while wrapping in zero-page
    CpuWord next =
        (cpu->addr_operand & 0xFF00) |
        (((cpu->addr_operand & 0x00FF) == 0xFF) ? 0x00 : (cpu->addr_operand & 0x00FF) + 1);

    CpuByte high = cpu_mem_read(cpu, next);
    cpu->ind_latch = ((CpuWord)high << 8) | cpu->ind_latch;

    cpu->cur_step = cpu_addr_pre_idx_indirect_read;
}

void cpu_addr_pre_idx_indirect_read(Cpu* cpu) {
    // Read from effective address
    cpu_mem_read(cpu, cpu->ind_latch);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_post_idx_indirect_low(Cpu* cpu) {
    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_post_idx_indirect_fetch_low;
}

void cpu_addr_post_idx_indirect_fetch_low(Cpu* cpu) {
    // Fetch the low effective address
    cpu->ind_latch = cpu_mem_read(cpu, cpu->addr_operand);

    cpu->cur_step = cpu_addr_post_idx_indirect_fetch_high;
}

void cpu_addr_post_idx_indirect_fetch_high(Cpu* cpu) {
    // Read second byte in effective address while wrapping in zero-page
    CpuWord next =
        (cpu->addr_operand & 0xFF00) |
        (((cpu->addr_operand & 0x00FF) == 0xFF) ? 0x00 : (cpu->addr_operand & 0x00FF) + 1);

    cpu_mem_read(cpu, next);

    // Add Y on latch, enter next cycle with high byte still on data bus
    cpu->ind_latch += cpu->y;

    cpu->cur_step = cpu_addr_post_idx_indirect_read1;
}

void cpu_addr_post_idx_indirect_read1(Cpu* cpu) {
    CpuByte high = cpu->data_bus;
    CpuWord low = cpu->ind_latch;

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
    cpu->ind_latch = ((CpuWord)(high + 1) << 8) | (low & 0xFF);

    cpu->cur_step = cpu_addr_post_idx_indirect_read2;
}

void cpu_addr_post_idx_indirect_read2(Cpu* cpu) {
    cpu_mem_read(cpu, cpu->ind_latch);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_post_idx_indirect_write_low(Cpu* cpu) {
    // Read the low byte
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_post_idx_indirect_write_fetch_low;
}

void cpu_addr_post_idx_indirect_write_fetch_low(Cpu* cpu) {
    // Fetch the low effective address
    cpu->ind_latch = cpu_mem_read(cpu, cpu->addr_operand);

    cpu->cur_step = cpu_addr_post_idx_indirect_write_fetch_high;
}

void cpu_addr_post_idx_indirect_write_fetch_high(Cpu* cpu) {
    // Read second byte in effective address while wrapping in zero-page
    CpuWord next =
        (cpu->addr_operand & 0xFF00) |
        (((cpu->addr_operand & 0x00FF) == 0xFF) ? 0x00 : (cpu->addr_operand & 0x00FF) + 1);

    cpu_mem_read(cpu, next);

    // Add Y on latch, enter next cycle with high byte still on data bus
    cpu->ind_latch += cpu->y;

    cpu->cur_step = cpu_addr_post_idx_indirect_write_read;
}

void cpu_addr_post_idx_indirect_write_read(Cpu* cpu) {
    CpuByte high = cpu->data_bus;
    CpuWord low = cpu->ind_latch;

    // Perform potentially invalid read
    // High byte is still on the address bus
    cpu_mem_read(cpu, ((CpuWord)(high << 8) | (low & 0xFF)));

    // Page boundary might have been crossed, second read performed must correct if necessary
    CpuByte overflow = (low >> 8) & 0x01;
    cpu->ind_latch = ((CpuWord)(high + overflow) << 8) | (low & 0xFF);

    cpu->cur_step = cpu_addr_post_idx_indirect_write_write;
}

void cpu_addr_post_idx_indirect_write_write(Cpu* cpu) {
    cpu_mem_read(cpu, cpu->ind_latch);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_relative_fetch_offset(Cpu* cpu) {
    cpu->addr_operand = cpu_mem_read(cpu, cpu->pc++);

    // Perform ALU logic to branch if necessary
    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Branch if branch flag was set by ALU logic
    cpu->cur_step = cpu_fetch_opcode;
    if (cpu->do_branch) {
        cpu->cur_step = cpu_addr_relative_dummy_read;
    }
}

void cpu_addr_relative_dummy_read(Cpu* cpu) {
    // Dummy read from PC immediately
    cpu_mem_read(cpu, cpu->pc);

    // Retrieve offset
    CpuByteSigned offset = (CpuByteSigned)(cpu->addr_operand & 0xFF);
    // Find target PC
    CpuWord new_pc = cpu->pc + offset;

    // Page boundary crossing check
    if ((cpu->pc & 0xFF00) == (new_pc & 0xFF00)) {
        // New PC lands on same page, no extra cycle needed
        cpu->pc = new_pc;
        cpu->cur_step = cpu_fetch_opcode;
        return;
    }

    // New PC was on adjacent page, fix with extra cycle
    cpu->addr_operand = (cpu->pc & 0xFF00) | (new_pc & 0x00FF);
    cpu->pc = new_pc;
    cpu->cur_step = cpu_addr_relative_invalid_read;
}

void cpu_addr_relative_invalid_read(Cpu* cpu) {
    // Read on invalid address and then fetch from correct address
    cpu_mem_read(cpu, cpu->addr_operand);
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_brk_dummy_read(Cpu* cpu) {
    // Dummy read
    cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_brk_push1;
}

void cpu_addr_brk_push1(Cpu* cpu) {
    // Push PCH onto the stack
    cpu_mem_write(cpu, CPU_MEM_STACK_START + cpu->sp--, cpu->pc >> 8);

    cpu->cur_step = cpu_addr_brk_push2;
}

void cpu_addr_brk_push2(Cpu* cpu) {
    // Push PCL onto the stack
    cpu_mem_write(cpu, CPU_MEM_STACK_START + cpu->sp--, cpu->pc & 0x00FF);

    cpu->cur_step = cpu_addr_brk_push3;
}

void cpu_addr_brk_push3(Cpu* cpu) {
    // Push SR onto the stack
    cpu_mem_write(cpu, CPU_MEM_STACK_START + cpu->sp--, cpu->sr | CPU_FLAG_B);

    // Disable interrupts to prevent nested IRQs
    cpu->sr |= CPU_FLAG_I;

    cpu->cur_step = cpu_addr_brk_vec_low_fetch;
}

void cpu_addr_brk_vec_low_fetch(Cpu* cpu) {
    // Fetch low byte from IRQ vector
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, CPU_MEM_IRQ_VEC_ADDR_L);

    cpu->cur_step = cpu_addr_brk_vec_high_fetch;
}

void cpu_addr_brk_vec_high_fetch(Cpu* cpu) {
    // Fetch high byte from IRQ vector
    cpu->pc = ((CpuWord)cpu_mem_read(cpu, CPU_MEM_IRQ_VEC_ADDR_H) << 8) | cpu->addr_operand;

    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_rti_dummy_read(Cpu* cpu) {
    // Dummy read
    cpu_mem_read(cpu, cpu->pc);
    cpu->cur_step = cpu_addr_rti_inc_sp;
}

void cpu_addr_rti_inc_sp(Cpu* cpu) {
    // Increment the stack pointer adter read on empty stack slot
    cpu_mem_read(cpu, CPU_MEM_STACK_START + cpu->sp++);
    cpu->cur_step = cpu_addr_rti_pull1;
}

void cpu_addr_rti_pull1(Cpu* cpu) {
    // Pull the SR off stack
    // Ignore the B flag and force U to 1
    cpu->sr = (cpu_mem_read(cpu, CPU_MEM_STACK_START + cpu->sp++) & ~CPU_FLAG_B) | CPU_FLAG_U;
    cpu->cur_step = cpu_addr_rti_pull2;
}

void cpu_addr_rti_pull2(Cpu* cpu) {
    // Pull PCL
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, CPU_MEM_STACK_START + cpu->sp++);
    cpu->cur_step = cpu_addr_rti_pull3;
}

void cpu_addr_rti_pull3(Cpu* cpu) {
    // Pull PCH
    cpu->pc = ((CpuWord)cpu_mem_read(cpu, CPU_MEM_STACK_START + cpu->sp) << 8) | cpu->addr_operand;
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_jsr_fetch_low(Cpu* cpu) {
    // Read operand
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_addr_jsr_dummy;
}

void cpu_addr_jsr_dummy(Cpu* cpu) {
    // Some internal operation. Just read stack
    cpu_mem_read(cpu, CPU_MEM_STACK_START + cpu->sp);
    cpu->cur_step = cpu_addr_jsr_push1;
}

void cpu_addr_jsr_push1(Cpu* cpu) {
    // Push PCH onto the stack
    cpu_mem_write(cpu, CPU_MEM_STACK_START + cpu->sp--, cpu->pc >> 8);
    cpu->cur_step = cpu_addr_jsr_push2;
}

void cpu_addr_jsr_push2(Cpu* cpu) {
    // Push PCL onto the stack
    cpu_mem_write(cpu, CPU_MEM_STACK_START + cpu->sp--, cpu->pc & 0x00FF);
    cpu->cur_step = cpu_addr_jsr_fetch_high;
}

void cpu_addr_jsr_fetch_high(Cpu* cpu) {
    // Read high byte to move PC to effective address
    cpu->pc = ((CpuWord)cpu_mem_read(cpu, cpu->pc) << 8) | cpu->addr_operand;
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_addr_rts_dummy1(Cpu* cpu) {
    // Dummy read on PC
    cpu_mem_read(cpu, cpu->pc);
    cpu->cur_step = cpu_addr_rts_dummy2;
}

void cpu_addr_rts_dummy2(Cpu* cpu) {
    // Dummy read on stack
    cpu_mem_read(cpu, CPU_MEM_STACK_START + cpu->sp++);
    cpu->cur_step = cpu_addr_rts_pull1;
}

void cpu_addr_rts_pull1(Cpu* cpu) {
    // Pull PCL off stack
    cpu->addr_operand = (CpuWord)cpu_mem_read(cpu, CPU_MEM_STACK_START + cpu->sp++);
    cpu->cur_step = cpu_addr_rts_pull2;
}

void cpu_addr_rts_pull2(Cpu* cpu) {
    // Pull PCH off stack
    cpu->pc = ((CpuWord)cpu_mem_read(cpu, CPU_MEM_STACK_START + cpu->sp) << 8) | cpu->addr_operand;
    cpu->cur_step = cpu_addr_rts_inc;
}

void cpu_addr_rts_inc(Cpu* cpu) {
    // Fix off by one in stack
    cpu_mem_read(cpu, cpu->pc++);
    cpu->cur_step = cpu_fetch_opcode;
}
