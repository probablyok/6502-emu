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
                              .alu_op = cpu_alu_ldy},
    [CPU_OP_AND_IMMEDIATE] = {.first_step = cpu_addr_immediate,
                              .alu_op = cpu_alu_and},
    [CPU_OP_ORA_IMMEDIATE] = {.first_step = cpu_addr_immediate,
                              .alu_op = cpu_alu_ora},
    [CPU_OP_EOR_IMMEDIATE] = {.first_step = cpu_addr_immediate,
                              .alu_op = cpu_alu_eor},
    [CPU_OP_ADC_IMMEDIATE] = {.first_step = cpu_addr_immediate,
                              .alu_op = cpu_alu_adc},
    [CPU_OP_CMP_IMMEDIATE] = {.first_step = cpu_addr_immediate,
                              .alu_op = cpu_alu_cmp},
    [CPU_OP_CPY_IMMEDIATE] = {.first_step = cpu_addr_immediate,
                              .alu_op = cpu_alu_cpy},
    [CPU_OP_CPX_IMMEDIATE] = {.first_step = cpu_addr_immediate,
                              .alu_op = cpu_alu_cpx},
    [CPU_OP_SBC_IMMEDIATE] = {.first_step = cpu_addr_immediate,
                              .alu_op = cpu_alu_sbc},
    [CPU_OP_SED_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_sed},
    [CPU_OP_CLD_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_cld},
    [CPU_OP_NOP_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_nop},
    [CPU_OP_CLC_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_clc},
    [CPU_OP_SEC_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_sec},
    [CPU_OP_DEY_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_dey},
    [CPU_OP_DEX_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_dex},
    [CPU_OP_TXA_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_txa},
    [CPU_OP_TYA_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_tya},
    [CPU_OP_TAX_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_tax},
    [CPU_OP_TAY_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_tay},
    [CPU_OP_CLV_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_clv},
    [CPU_OP_INY_IMPLIED] = {.first_step = cpu_addr_implied,
                            .alu_op = cpu_alu_iny},
};

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

/// @brief Set/unset the carry flag in the status register based on true or
/// false condition provided
/// @param cpu The emulated 6502 CPU
/// @param condition The condition used to set the carry flag
static void cpu_status_reg_update_carry(Cpu *cpu, int condition) {
    cpu->sr = condition ? ((cpu->sr) | CPU_FLAG_C) : ((cpu->sr) & ~CPU_FLAG_C);
}

/// @brief Set/unset the overflow flag in the status register based on true or
/// false condition provided
/// @param cpu The emulated 6502 CPU
/// @param condition The condition used to set the overflow flag
static void cpu_status_reg_update_overflow(Cpu *cpu, int condition) {
    cpu->sr = condition ? ((cpu->sr) | CPU_FLAG_V) : ((cpu->sr) & ~CPU_FLAG_V);
}

/// @brief Set/unset the BCD flag in the status register based on input
/// @param cpu The emulated 6502 CPU
/// @param condition The data to set
static void cpu_status_reg_update_decimal(Cpu *cpu, int data) {
    cpu->sr = (data) ? ((cpu->sr) | CPU_FLAG_D) : ((cpu->sr) & ~CPU_FLAG_D);
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

void cpu_addr_implied(Cpu *cpu) {
    // Dummy read byte after opcode
    cpu_mem_read(cpu, cpu->pc);

    if (cpu->pending_alu) {
        cpu->pending_alu(cpu);
    }

    // Operation complete, fetch new opcode
    cpu->cur_step = cpu_fetch_opcode;
}

void cpu_alu_lda(Cpu *cpu) {
    cpu->a = cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->a);
    cpu_status_reg_update_zero(cpu, cpu->a);
}

void cpu_alu_ldx(Cpu *cpu) {
    cpu->x = cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->x);
    cpu_status_reg_update_zero(cpu, cpu->x);
}

void cpu_alu_ldy(Cpu *cpu) {
    cpu->y = cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->y);
    cpu_status_reg_update_zero(cpu, cpu->y);
}

void cpu_alu_and(Cpu *cpu) {
    cpu->a = cpu->a & cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->a);
    cpu_status_reg_update_zero(cpu, cpu->a);
}

void cpu_alu_ora(Cpu *cpu) {
    cpu->a = cpu->a | cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->a);
    cpu_status_reg_update_zero(cpu, cpu->a);
}

void cpu_alu_eor(Cpu *cpu) {
    cpu->a = cpu->a ^ cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, cpu->a);
    cpu_status_reg_update_zero(cpu, cpu->a);
}

void cpu_alu_adc(Cpu *cpu) {
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

        cpu_status_reg_update_overflow(cpu, ~(old_a ^ operand) &
                                                (old_a ^ (a & 0xFF)) & 0x80);

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
    cpu_status_reg_update_overflow(
        cpu, ~(old_a ^ operand) & (old_a ^ (word_result_bin & 0xFF)) & 0x80);

    // Update accumulator and carry
    cpu->a = word_result_bin & 0xFF;
    cpu_status_reg_update_carry(cpu, word_result_bin > 0xFF);
}

void cpu_alu_cmp(Cpu *cpu) {
    CpuWord compare_result = cpu->a - cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, (CpuByte)compare_result);
    cpu_status_reg_update_zero(cpu, (CpuByte)compare_result);
    cpu_status_reg_update_carry(cpu, cpu->a >= cpu->data_bus);
}

void cpu_alu_cpy(Cpu *cpu) {
    CpuWord comapre_result = cpu->y - cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, (CpuByte)comapre_result);
    cpu_status_reg_update_zero(cpu, (CpuByte)comapre_result);
    cpu_status_reg_update_carry(cpu, cpu->y >= cpu->data_bus);
}

void cpu_alu_cpx(Cpu *cpu) {
    CpuWord compare_result = cpu->x - cpu->data_bus;
    // Update status register
    cpu_status_reg_update_negative(cpu, (CpuByte)compare_result);
    cpu_status_reg_update_zero(cpu, (CpuByte)compare_result);
    cpu_status_reg_update_carry(cpu, cpu->x >= cpu->data_bus);
}

void cpu_alu_sbc(Cpu *cpu) {
    CpuByte old_a = cpu->a;
    CpuByte operand = cpu->data_bus;
    CpuByte carry_in = !!(cpu->sr & CPU_FLAG_C);

    // 16-bit binary result
    CpuWord word_result_bin = old_a - operand - (1 - carry_in);

    // Status flags
    cpu_status_reg_update_zero(cpu, word_result_bin & 0xFF);
    cpu_status_reg_update_negative(cpu, word_result_bin & 0xFF);
    cpu_status_reg_update_overflow(
        cpu, (old_a ^ operand) & (old_a ^ (word_result_bin & 0xFF)) & 0x80);
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

void cpu_alu_sed(Cpu *cpu) {
    // Update status register
    cpu_status_reg_update_decimal(cpu, 1);
}

void cpu_alu_cld(Cpu *cpu) {
    // Update status register
    cpu_status_reg_update_decimal(cpu, 0);
}

void cpu_alu_nop(Cpu *cpu) {
    cpu = (Cpu *)cpu;
    return; // Do nothing
}

void cpu_alu_clc(Cpu *cpu) {
    // Update status register
    cpu_status_reg_update_carry(cpu, 0);
}

void cpu_alu_sec(Cpu *cpu) {
    // Update status register
    cpu_status_reg_update_carry(cpu, 1);
}

void cpu_alu_dey(Cpu *cpu) {
    cpu->y = cpu->y - 1;
    // Update status registers
    cpu_status_reg_update_zero(cpu, cpu->y);
    cpu_status_reg_update_negative(cpu, cpu->y);
}

void cpu_alu_dex(Cpu *cpu) {
    cpu->x = cpu->x - 1;
    // Update status registers
    cpu_status_reg_update_zero(cpu, cpu->x);
    cpu_status_reg_update_negative(cpu, cpu->x);
}

void cpu_alu_txa(Cpu *cpu) {
    cpu->a = cpu->x;
    cpu_status_reg_update_zero(cpu, cpu->x);
    cpu_status_reg_update_negative(cpu, cpu->x);
}

void cpu_alu_tya(Cpu *cpu) {
    cpu->a = cpu->y;
    cpu_status_reg_update_zero(cpu, cpu->y);
    cpu_status_reg_update_negative(cpu, cpu->y);
}

void cpu_alu_tax(Cpu *cpu) {
    cpu->x = cpu->a;
    cpu_status_reg_update_zero(cpu, cpu->x);
    cpu_status_reg_update_negative(cpu, cpu->x);
}

void cpu_alu_tay(Cpu *cpu) {
    cpu->y = cpu->a;
    cpu_status_reg_update_zero(cpu, cpu->y);
    cpu_status_reg_update_negative(cpu, cpu->y);
}

void cpu_alu_clv(Cpu *cpu) {
    // Set status flag
    cpu_status_reg_update_overflow(cpu, 0);
}

void cpu_alu_iny(Cpu *cpu) {
    cpu->y = cpu->y + 1;
    // Set status flag
    cpu_status_reg_update_zero(cpu, cpu->y);
    cpu_status_reg_update_negative(cpu, cpu->y);
}