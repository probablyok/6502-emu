#include "cpu_state.h"
#include "cpu_addr.h"
#include "cpu_alu.h"
#include "cpu_macros.h"

#include <stdlib.h>

const CpuInstruction INSTRUCTION_TABLE[256] = {
    [CPU_OP_LDA_IMMEDIATE] = {.first_step = cpu_addr_immediate, .alu_op = cpu_alu_lda},
    [CPU_OP_LDX_IMMEDIATE] = {.first_step = cpu_addr_immediate, .alu_op = cpu_alu_ldx},
    [CPU_OP_LDY_IMMEDIATE] = {.first_step = cpu_addr_immediate, .alu_op = cpu_alu_ldy},
    [CPU_OP_AND_IMMEDIATE] = {.first_step = cpu_addr_immediate, .alu_op = cpu_alu_and},
    [CPU_OP_ORA_IMMEDIATE] = {.first_step = cpu_addr_immediate, .alu_op = cpu_alu_ora},
    [CPU_OP_EOR_IMMEDIATE] = {.first_step = cpu_addr_immediate, .alu_op = cpu_alu_eor},
    [CPU_OP_ADC_IMMEDIATE] = {.first_step = cpu_addr_immediate, .alu_op = cpu_alu_adc},
    [CPU_OP_CMP_IMMEDIATE] = {.first_step = cpu_addr_immediate, .alu_op = cpu_alu_cmp},
    [CPU_OP_CPY_IMMEDIATE] = {.first_step = cpu_addr_immediate, .alu_op = cpu_alu_cpy},
    [CPU_OP_CPX_IMMEDIATE] = {.first_step = cpu_addr_immediate, .alu_op = cpu_alu_cpx},
    [CPU_OP_SBC_IMMEDIATE] = {.first_step = cpu_addr_immediate, .alu_op = cpu_alu_sbc},
    [CPU_OP_SED_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_sed},
    [CPU_OP_CLD_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_cld},
    [CPU_OP_NOP_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_nop},
    [CPU_OP_CLC_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_clc},
    [CPU_OP_SEC_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_sec},
    [CPU_OP_DEY_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_dey},
    [CPU_OP_DEX_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_dex},
    [CPU_OP_TXA_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_txa},
    [CPU_OP_TYA_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_tya},
    [CPU_OP_TAX_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_tax},
    [CPU_OP_TAY_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_tay},
    [CPU_OP_CLV_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_clv},
    [CPU_OP_INX_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_inx},
    [CPU_OP_INY_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_iny},
    [CPU_OP_PHP_IMPLIED] = {.first_step = cpu_wait_php_pha, .alu_op = cpu_alu_php},
    [CPU_OP_PHA_IMPLIED] = {.first_step = cpu_wait_php_pha, .alu_op = cpu_alu_pha},
    [CPU_OP_PLP_IMPLIED] = {.first_step = cpu_wait1_plp_pla, .alu_op = cpu_alu_plp},
    [CPU_OP_PLA_IMPLIED] = {.first_step = cpu_wait1_plp_pla, .alu_op = cpu_alu_pla},
    [CPU_OP_TXS_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_txs},
    [CPU_OP_TSX_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_tsx},
    [CPU_OP_CLI_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_cli},
    [CPU_OP_SEI_IMPLIED] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_sei},
    [CPU_OP_ASL_ACCUMULATOR] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_asl},
    [CPU_OP_ROL_ACCUMULATOR] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_rol},
    [CPU_OP_LSR_ACCUMULATOR] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_lsr},
    [CPU_OP_ROR_ACCUMULATOR] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_ror},
    [CPU_OP_ORA_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_ora},
    [CPU_OP_AND_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_and},
    [CPU_OP_EOR_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_eor},
    [CPU_OP_ADC_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_adc},
    [CPU_OP_LDA_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_lda},
    [CPU_OP_CMP_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_cmp},
    [CPU_OP_SBC_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_sbc},
    [CPU_OP_LDY_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_ldy},
    [CPU_OP_CPY_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_cpy},
    [CPU_OP_CPX_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_cpx},
    [CPU_OP_LDX_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_ldx},
    [CPU_OP_BIT_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_bit},
    [CPU_OP_STY_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_sty},
    [CPU_OP_STA_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_sta},
    [CPU_OP_STX_ABSOLUTE] = {.first_step = cpu_addr_absolute_low, .alu_op = cpu_alu_stx},
    [CPU_OP_ASL_ABSOLUTE] = {.first_step = cpu_addr_absolute_rmw_low, .alu_op = cpu_alu_asl_mem},
    [CPU_OP_ROL_ABSOLUTE] = {.first_step = cpu_addr_absolute_rmw_low, .alu_op = cpu_alu_rol_mem},
    [CPU_OP_LSR_ABSOLUTE] = {.first_step = cpu_addr_absolute_rmw_low, .alu_op = cpu_alu_lsr_mem},
    [CPU_OP_ROR_ABSOLUTE] = {.first_step = cpu_addr_absolute_rmw_low, .alu_op = cpu_alu_ror_mem},
    [CPU_OP_INC_ABSOLUTE] = {.first_step = cpu_addr_absolute_rmw_low, .alu_op = cpu_alu_inc},
    [CPU_OP_DEC_ABSOLUTE] = {.first_step = cpu_addr_absolute_rmw_low, .alu_op = cpu_alu_dec},
};

void cpu_status_reg_update_zero(Cpu* cpu, CpuByte data) {
    cpu->sr = (data == 0) ? ((cpu->sr) | CPU_FLAG_Z) : ((cpu->sr) & ~CPU_FLAG_Z);
}

void cpu_status_reg_update_negative(Cpu* cpu, CpuByte data) {
    cpu->sr = (data & 0x80) ? ((cpu->sr) | CPU_FLAG_N) : ((cpu->sr) & ~CPU_FLAG_N);
}

void cpu_status_reg_update_carry(Cpu* cpu, int condition) {
    cpu->sr = condition ? ((cpu->sr) | CPU_FLAG_C) : ((cpu->sr) & ~CPU_FLAG_C);
}

void cpu_status_reg_update_overflow(Cpu* cpu, int condition) {
    cpu->sr = condition ? ((cpu->sr) | CPU_FLAG_V) : ((cpu->sr) & ~CPU_FLAG_V);
}

void cpu_status_reg_update_decimal(Cpu* cpu, int data) {
    cpu->sr = (data) ? ((cpu->sr) | CPU_FLAG_D) : ((cpu->sr) & ~CPU_FLAG_D);
}

void cpu_status_reg_update_interrupt_disable(Cpu* cpu, int data) {
    cpu->sr = (data) ? ((cpu->sr) | CPU_FLAG_I) : ((cpu->sr) & ~CPU_FLAG_I);
}

int cpu_inst_implemented(CpuByte opcode) { return INSTRUCTION_TABLE[opcode].first_step == NULL; }

void cpu_tick(Cpu* cpu) { cpu->cur_step(cpu); }
