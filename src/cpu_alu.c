#include "cpu_alu.h"
#include "cpu_addr.h"
#include "cpu_macros.h"
#include "cpu_mem.h"
#include "cpu_state.h"

void cpu_alu_lda(Cpu* cpu) {
    cpu->a = cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->a);
    cpu_status_reg_update_zero(cpu, cpu->a);
}

void cpu_alu_ldx(Cpu* cpu) {
    cpu->x = cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->x);
    cpu_status_reg_update_zero(cpu, cpu->x);
}

void cpu_alu_ldy(Cpu* cpu) {
    cpu->y = cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->y);
    cpu_status_reg_update_zero(cpu, cpu->y);
}

void cpu_alu_and(Cpu* cpu) {
    cpu->a = cpu->a & cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->a);
    cpu_status_reg_update_zero(cpu, cpu->a);
}

void cpu_alu_ora(Cpu* cpu) {
    cpu->a = cpu->a | cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->a);
    cpu_status_reg_update_zero(cpu, cpu->a);
}

void cpu_alu_eor(Cpu* cpu) {
    cpu->a = cpu->a ^ cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->a);
    cpu_status_reg_update_zero(cpu, cpu->a);
}

void cpu_alu_adc(Cpu* cpu) {
    CpuByte old_a = cpu->a;
    CpuByte operand = cpu->data_bus;
    CpuByte carry_in = !!(cpu->sr & CPU_FLAG_C);

    // 16-bit binary result
    CpuWord word_result_bin = old_a + operand + carry_in;

    // Zero flag uses raw binary result
    cpu_status_reg_update_zero(cpu, word_result_bin & 0xFF);

    // BCD specific instructions
    if (cpu->sr & CPU_FLAG_D) {
        // Bruce Clark's canonical BCD algorithm
        CpuWord al = (old_a & 0x0F) + (operand & 0x0F) + carry_in;
        // Apply offset
        if (al >= 0x0A) {
            al = ((al + 0x06) & 0x0F) + 0x10;
        }

        CpuWord a = (old_a & 0xF0) + (operand & 0xF0) + al;

        // N and V flags use the intermediate result
        cpu_status_reg_update_negative(cpu, a & 0xFF);

        cpu_status_reg_update_overflow(cpu, ~(old_a ^ operand) & (old_a ^ (a & 0xFF)) & 0x80);

        // Apply offset
        if (a >= 0xA0) {
            a += 0x60;
        }

        // Update accumulator and carry
        cpu->a = a & 0xFF;
        cpu_status_reg_update_carry(cpu, a >= 0x100);
        return;
    }
    // Update N and V flags with raw binary result
    cpu_status_reg_update_negative(cpu, word_result_bin & 0xFF);
    cpu_status_reg_update_overflow(cpu,
                                   ~(old_a ^ operand) & (old_a ^ (word_result_bin & 0xFF)) & 0x80);

    // Update accumulator and carry
    cpu->a = word_result_bin & 0xFF;
    cpu_status_reg_update_carry(cpu, word_result_bin > 0xFF);
}

void cpu_alu_cmp(Cpu* cpu) {
    CpuWord compare_result = cpu->a - cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, (CpuByte)compare_result);
    cpu_status_reg_update_zero(cpu, (CpuByte)compare_result);
    cpu_status_reg_update_carry(cpu, cpu->a >= cpu->data_bus);
}

void cpu_alu_cpy(Cpu* cpu) {
    CpuWord comapre_result = cpu->y - cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, (CpuByte)comapre_result);
    cpu_status_reg_update_zero(cpu, (CpuByte)comapre_result);
    cpu_status_reg_update_carry(cpu, cpu->y >= cpu->data_bus);
}

void cpu_alu_cpx(Cpu* cpu) {
    CpuWord compare_result = cpu->x - cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, (CpuByte)compare_result);
    cpu_status_reg_update_zero(cpu, (CpuByte)compare_result);
    cpu_status_reg_update_carry(cpu, cpu->x >= cpu->data_bus);
}

void cpu_alu_sbc(Cpu* cpu) {
    CpuByte old_a = cpu->a;
    CpuByte operand = cpu->data_bus;
    CpuByte carry_in = !!(cpu->sr & CPU_FLAG_C);

    // 16-bit binary result
    CpuWord word_result_bin = old_a - operand - (1 - carry_in);

    // Status flags
    cpu_status_reg_update_zero(cpu, word_result_bin & 0xFF);
    cpu_status_reg_update_negative(cpu, word_result_bin & 0xFF);
    cpu_status_reg_update_overflow(cpu,
                                   (old_a ^ operand) & (old_a ^ (word_result_bin & 0xFF)) & 0x80);
    // Inverted borrow (carry only 1 on no underflow)
    cpu_status_reg_update_carry(cpu, word_result_bin < 0x100);

    // BCD specific instructions
    if (cpu->sr & CPU_FLAG_D) {
        // Bruce Clark's canonical BCD SBC algorithm
        int al = (old_a & 0x0F) - (operand & 0x0F) - (1 - carry_in);
        if (al < 0) {
            al = ((al - 0x06) & 0x0F) - 0x10;
        }

        int a = (old_a & 0xF0) - (operand & 0xF0) + al;
        if (a < 0) {
            a -= 0x60;
        }

        cpu->a = a & 0xFF;
        return;
    }
    // Update accumulator and carry
    cpu->a = word_result_bin & 0xFF;
}

void cpu_alu_sed(Cpu* cpu) {
    // Update status register
    cpu_status_reg_update_decimal(cpu, 1);
}

void cpu_alu_cld(Cpu* cpu) {
    // Update status register
    cpu_status_reg_update_decimal(cpu, 0);
}

void cpu_alu_nop(Cpu* cpu) {
    cpu = (Cpu*)cpu;
    return; // Do nothing
}

void cpu_alu_clc(Cpu* cpu) {
    // Update status register
    cpu_status_reg_update_carry(cpu, 0);
}

void cpu_alu_sec(Cpu* cpu) {
    // Update status register
    cpu_status_reg_update_carry(cpu, 1);
}

void cpu_alu_dey(Cpu* cpu) {
    cpu->y = cpu->y - 1;
    // Update status registers
    cpu_status_reg_update_zero(cpu, cpu->y);
    cpu_status_reg_update_negative(cpu, cpu->y);
}

void cpu_alu_dex(Cpu* cpu) {
    cpu->x = cpu->x - 1;
    // Update status registers
    cpu_status_reg_update_zero(cpu, cpu->x);
    cpu_status_reg_update_negative(cpu, cpu->x);
}

void cpu_alu_txa(Cpu* cpu) {
    cpu->a = cpu->x;
    cpu_status_reg_update_zero(cpu, cpu->x);
    cpu_status_reg_update_negative(cpu, cpu->x);
}

void cpu_alu_tya(Cpu* cpu) {
    cpu->a = cpu->y;
    cpu_status_reg_update_zero(cpu, cpu->y);
    cpu_status_reg_update_negative(cpu, cpu->y);
}

void cpu_alu_tax(Cpu* cpu) {
    cpu->x = cpu->a;
    cpu_status_reg_update_zero(cpu, cpu->x);
    cpu_status_reg_update_negative(cpu, cpu->x);
}

void cpu_alu_tay(Cpu* cpu) {
    cpu->y = cpu->a;
    cpu_status_reg_update_zero(cpu, cpu->y);
    cpu_status_reg_update_negative(cpu, cpu->y);
}

void cpu_alu_clv(Cpu* cpu) {
    // Set status flag
    cpu_status_reg_update_overflow(cpu, 0);
}

void cpu_alu_inx(Cpu* cpu) {
    cpu->x = cpu->x + 1;
    // Set status flag
    cpu_status_reg_update_zero(cpu, cpu->x);
    cpu_status_reg_update_negative(cpu, cpu->x);
}

void cpu_alu_iny(Cpu* cpu) {
    cpu->y = cpu->y + 1;
    // Set status flag
    cpu_status_reg_update_zero(cpu, cpu->y);
    cpu_status_reg_update_negative(cpu, cpu->y);
}

void cpu_wait_php_pha(Cpu* cpu) {
    // Read byte after opcode
    cpu_mem_read(cpu, cpu->pc);

    // Dummy wait done
    cpu->cur_step = cpu_wait_do_alu;
}

void cpu_wait_do_alu(Cpu* cpu) {
    cpu->pending_alu(cpu);            // Do push/pull
    cpu->cur_step = cpu_fetch_opcode; // Back to getting next opcode
}

void cpu_alu_php(Cpu* cpu) {
    // Push status register onto stack with bit 5 and BREAK set to 1
    CpuWord stack_pos = CPU_MEM_STACK_START + cpu->sp--;
    cpu_mem_write(cpu, stack_pos, cpu->sr | CPU_FLAG_B | CPU_FLAG_U);
}

void cpu_alu_pha(Cpu* cpu) {
    // Push status register onto stack with bit 5 and BREAK set to 1
    CpuWord stack_pos = CPU_MEM_STACK_START + cpu->sp--;
    cpu_mem_write(cpu, stack_pos, cpu->a);
}

void cpu_wait1_plp_pla(Cpu* cpu) {
    // Dummy read read on next instruction byte
    cpu_mem_read(cpu, cpu->pc);
    cpu->cur_step = cpu_wait2_plp_pla;
}

void cpu_wait2_plp_pla(Cpu* cpu) {
    // Dummy read on stack pointer with increment
    CpuWord stack_pos = CPU_MEM_STACK_START + cpu->sp++;
    cpu_mem_read(cpu, stack_pos);
    cpu->cur_step = cpu_wait_do_alu;
}

void cpu_alu_plp(Cpu* cpu) {
    // Pull new status register from stack
    CpuWord stack_pos = CPU_MEM_STACK_START + cpu->sp;
    CpuByte stack_read = cpu_mem_read(cpu, stack_pos);

    // Force bit 5 to 1 and clear BREAK
    cpu->sr = (stack_read & ~CPU_FLAG_B) | CPU_FLAG_U;
}

void cpu_alu_pla(Cpu* cpu) {
    // Pull new status register from stack
    CpuWord stack_pos = CPU_MEM_STACK_START + cpu->sp;
    CpuByte stack_read = cpu_mem_read(cpu, stack_pos);

    // Read into accumulator from stack
    cpu->a = stack_read;

    // Update stack register
    cpu_status_reg_update_zero(cpu, cpu->a);
    cpu_status_reg_update_negative(cpu, cpu->a);
}

void cpu_alu_txs(Cpu* cpu) {
    // Set X register to stack pointer
    cpu->sp = cpu->x;
}

void cpu_alu_tsx(Cpu* cpu) {
    // Set X register to stack pointer
    cpu->x = cpu->sp;

    // Update stack register
    cpu_status_reg_update_zero(cpu, cpu->x);
    cpu_status_reg_update_negative(cpu, cpu->x);
}

void cpu_alu_cli(Cpu* cpu) {
    // Set status register
    cpu_status_reg_update_interrupt_disable(cpu, 0);
}

void cpu_alu_sei(Cpu* cpu) {
    // Set status register
    cpu_status_reg_update_interrupt_disable(cpu, 1);
}

void cpu_alu_asl(Cpu* cpu) {
    CpuByte old = cpu->a;
    cpu->a = cpu->a << 1;

    // Set status register
    cpu_status_reg_update_carry(cpu, old & 0x80);
    cpu_status_reg_update_zero(cpu, cpu->a);
    cpu_status_reg_update_negative(cpu, cpu->a);
}

void cpu_alu_rol(Cpu* cpu) {
    CpuByte old = cpu->a;
    cpu->a = cpu->a << 1;
    cpu->a = cpu->a | !!(cpu->sr & CPU_FLAG_C);

    // Set status register
    cpu_status_reg_update_carry(cpu, old & 0x80);
    cpu_status_reg_update_zero(cpu, cpu->a);
    cpu_status_reg_update_negative(cpu, cpu->a);
}

void cpu_alu_lsr(Cpu* cpu) {
    CpuByte old = cpu->a;
    cpu->a = cpu->a >> 1;

    // Set status register
    cpu_status_reg_update_carry(cpu, old & 0x01);
    cpu_status_reg_update_zero(cpu, cpu->a);
    cpu_status_reg_update_negative(cpu, cpu->a);
}

void cpu_alu_ror(Cpu* cpu) {
    CpuByte old = cpu->a;
    cpu->a = cpu->a >> 1;
    cpu->a = cpu->a | ((cpu->sr & CPU_FLAG_C) ? 0x80 : 0x00);

    // Set status register
    cpu_status_reg_update_carry(cpu, old & 0x01);
    cpu_status_reg_update_zero(cpu, cpu->a);
    cpu_status_reg_update_negative(cpu, cpu->a);
}

void cpu_alu_asl_mem(Cpu* cpu) {
    CpuByte old = cpu->data_bus;
    cpu_mem_write(cpu, cpu->addr_bus, cpu->data_bus << 1);

    // Set status register
    cpu_status_reg_update_carry(cpu, old & 0x80);
    cpu_status_reg_update_zero(cpu, cpu->data_bus);
    cpu_status_reg_update_negative(cpu, cpu->data_bus);
}

void cpu_alu_rol_mem(Cpu* cpu) {
    CpuByte old = cpu->data_bus;
    CpuByte temp = cpu->data_bus << 1;
    cpu_mem_write(cpu, cpu->addr_bus, temp | !!(cpu->sr & CPU_FLAG_C));

    // Set status register
    cpu_status_reg_update_carry(cpu, old & 0x80);
    cpu_status_reg_update_zero(cpu, cpu->data_bus);
    cpu_status_reg_update_negative(cpu, cpu->data_bus);
}

void cpu_alu_lsr_mem(Cpu* cpu) {
    CpuByte old = cpu->data_bus;
    cpu_mem_write(cpu, cpu->addr_bus, cpu->data_bus >> 1);

    // Set status register
    cpu_status_reg_update_carry(cpu, old & 0x01);
    cpu_status_reg_update_zero(cpu, cpu->data_bus);
    cpu_status_reg_update_negative(cpu, cpu->data_bus);
}

void cpu_alu_ror_mem(Cpu* cpu) {
    CpuByte old = cpu->data_bus;
    CpuByte temp = cpu->data_bus >> 1;
    cpu_mem_write(cpu, cpu->addr_bus, temp | ((cpu->sr & CPU_FLAG_C) ? 0x80 : 0x00));

    // Set status register
    cpu_status_reg_update_carry(cpu, old & 0x01);
    cpu_status_reg_update_zero(cpu, cpu->data_bus);
    cpu_status_reg_update_negative(cpu, cpu->data_bus);
}

void cpu_alu_bit(Cpu* cpu) {
    // AND the accumulator and memory value
    CpuByte result = cpu->a & cpu->data_bus;

    // Set status registers
    cpu_status_reg_update_negative(cpu, cpu->data_bus);            // Result bit 7
    cpu_status_reg_update_overflow(cpu, cpu->data_bus & (1 << 6)); // Result bit 6
    cpu_status_reg_update_zero(cpu, result);                       // Result itself
}

void cpu_alu_inc(Cpu* cpu) {
    // Increment the memory by 1
    cpu_mem_write(cpu, cpu->addr_bus, cpu->data_bus + 1);

    // Set status registers
    cpu_status_reg_update_negative(cpu, cpu->data_bus);
    cpu_status_reg_update_zero(cpu, cpu->data_bus);
}
