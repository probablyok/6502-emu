#include "cpu_state.h"
#include "cpu_addr.h"
#include "cpu_alu.h"
#include "cpu_macros.h"

#include <stdlib.h>

const CpuInstruction INSTRUCTION_TABLE[256] = {
    // IMMEDIATE ADDRESSING
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

    // IMPLIED ADDRESSING
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

    // ACCUMULATOR ADDRESSING
    [CPU_OP_ASL_ACCUMULATOR] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_asl},
    [CPU_OP_ROL_ACCUMULATOR] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_rol},
    [CPU_OP_LSR_ACCUMULATOR] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_lsr},
    [CPU_OP_ROR_ACCUMULATOR] = {.first_step = cpu_addr_implied, .alu_op = cpu_alu_ror},

    // INDIRECT ADDRESSING
    [CPU_OP_JMP_INDIRECT] = {.first_step = cpu_addr_indirect_jmp_low, .alu_op = cpu_alu_jmp},

    // ABSOLUTE ADDRESSING
    [CPU_OP_JMP_ABSOLUTE] = {.first_step = cpu_addr_absolute_jmp_low, .alu_op = cpu_alu_jmp},
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

    // ZERO-PAGE ADDERSSING
    [CPU_OP_ORA_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_ora},
    [CPU_OP_AND_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_and},
    [CPU_OP_EOR_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_eor},
    [CPU_OP_ADC_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_adc},
    [CPU_OP_LDA_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_lda},
    [CPU_OP_CMP_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_cmp},
    [CPU_OP_SBC_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_sbc},
    [CPU_OP_LDY_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_ldy},
    [CPU_OP_CPY_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_cpy},
    [CPU_OP_CPX_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_cpx},
    [CPU_OP_LDX_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_ldx},
    [CPU_OP_BIT_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_bit},
    [CPU_OP_STY_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_sty},
    [CPU_OP_STA_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_sta},
    [CPU_OP_STX_ZERO_PAGE] = {.first_step = cpu_addr_zpg_low, .alu_op = cpu_alu_stx},
    [CPU_OP_ASL_ZERO_PAGE] = {.first_step = cpu_addr_zpg_rmw_low, .alu_op = cpu_alu_asl_mem},
    [CPU_OP_ROL_ZERO_PAGE] = {.first_step = cpu_addr_zpg_rmw_low, .alu_op = cpu_alu_rol_mem},
    [CPU_OP_LSR_ZERO_PAGE] = {.first_step = cpu_addr_zpg_rmw_low, .alu_op = cpu_alu_lsr_mem},
    [CPU_OP_ROR_ZERO_PAGE] = {.first_step = cpu_addr_zpg_rmw_low, .alu_op = cpu_alu_ror_mem},
    [CPU_OP_INC_ZERO_PAGE] = {.first_step = cpu_addr_zpg_rmw_low, .alu_op = cpu_alu_inc},
    [CPU_OP_DEC_ZERO_PAGE] = {.first_step = cpu_addr_zpg_rmw_low, .alu_op = cpu_alu_dec},

    // ABSOLUTE,X INDEXED ADDRESING
    [CPU_OP_ORA_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_low_x, .alu_op = cpu_alu_ora},
    [CPU_OP_AND_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_low_x, .alu_op = cpu_alu_and},
    [CPU_OP_EOR_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_low_x, .alu_op = cpu_alu_eor},
    [CPU_OP_ADC_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_low_x, .alu_op = cpu_alu_adc},
    [CPU_OP_LDY_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_low_x, .alu_op = cpu_alu_ldy},
    [CPU_OP_LDA_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_low_x, .alu_op = cpu_alu_lda},
    [CPU_OP_CMP_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_low_x, .alu_op = cpu_alu_cmp},
    [CPU_OP_SBC_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_low_x, .alu_op = cpu_alu_sbc},
    [CPU_OP_STA_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_write_low_x,
                                  .alu_op = cpu_alu_sta},
    [CPU_OP_ASL_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_rmw_low_x,
                                  .alu_op = cpu_alu_asl_mem},
    [CPU_OP_ROL_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_rmw_low_x,
                                  .alu_op = cpu_alu_rol_mem},
    [CPU_OP_LSR_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_rmw_low_x,
                                  .alu_op = cpu_alu_lsr_mem},
    [CPU_OP_ROR_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_rmw_low_x,
                                  .alu_op = cpu_alu_ror_mem},
    [CPU_OP_INC_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_rmw_low_x, .alu_op = cpu_alu_inc},
    [CPU_OP_DEC_ABS_X_INDEXED] = {.first_step = cpu_addr_abs_idx_rmw_low_x, .alu_op = cpu_alu_dec},

    // ABSOLUTE,Y INDEXED ADDRESSING
    [CPU_OP_ORA_ABS_Y_INDEXED] = {.first_step = cpu_addr_abs_idx_low_y, .alu_op = cpu_alu_ora},
    [CPU_OP_AND_ABS_Y_INDEXED] = {.first_step = cpu_addr_abs_idx_low_y, .alu_op = cpu_alu_and},
    [CPU_OP_EOR_ABS_Y_INDEXED] = {.first_step = cpu_addr_abs_idx_low_y, .alu_op = cpu_alu_eor},
    [CPU_OP_ADC_ABS_Y_INDEXED] = {.first_step = cpu_addr_abs_idx_low_y, .alu_op = cpu_alu_adc},
    [CPU_OP_LDA_ABS_Y_INDEXED] = {.first_step = cpu_addr_abs_idx_low_y, .alu_op = cpu_alu_lda},
    [CPU_OP_CMP_ABS_Y_INDEXED] = {.first_step = cpu_addr_abs_idx_low_y, .alu_op = cpu_alu_cmp},
    [CPU_OP_SBC_ABS_Y_INDEXED] = {.first_step = cpu_addr_abs_idx_low_y, .alu_op = cpu_alu_sbc},
    [CPU_OP_STA_ABS_Y_INDEXED] = {.first_step = cpu_addr_abs_idx_write_low_y,
                                  .alu_op = cpu_alu_sta},

    // ZERO-PAGE,X INDEXED ADDRESSING
    [CPU_OP_ORA_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_x, .alu_op = cpu_alu_ora},
    [CPU_OP_AND_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_x, .alu_op = cpu_alu_and},
    [CPU_OP_EOR_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_x, .alu_op = cpu_alu_eor},
    [CPU_OP_ADC_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_x, .alu_op = cpu_alu_adc},
    [CPU_OP_LDY_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_x, .alu_op = cpu_alu_ldy},
    [CPU_OP_LDA_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_x, .alu_op = cpu_alu_lda},
    [CPU_OP_CMP_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_x, .alu_op = cpu_alu_cmp},
    [CPU_OP_SBC_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_x, .alu_op = cpu_alu_sbc},
    [CPU_OP_STY_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_x, .alu_op = cpu_alu_sty},
    [CPU_OP_STA_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_x, .alu_op = cpu_alu_sta},
    [CPU_OP_ASL_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_rmw_low_x,
                                  .alu_op = cpu_alu_asl_mem},
    [CPU_OP_ROL_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_rmw_low_x,
                                  .alu_op = cpu_alu_rol_mem},
    [CPU_OP_LSR_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_rmw_low_x,
                                  .alu_op = cpu_alu_lsr_mem},
    [CPU_OP_ROR_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_rmw_low_x,
                                  .alu_op = cpu_alu_ror_mem},
    [CPU_OP_INC_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_rmw_low_x, .alu_op = cpu_alu_inc},
    [CPU_OP_DEC_ZPG_X_INDEXED] = {.first_step = cpu_addr_zpg_idx_rmw_low_x, .alu_op = cpu_alu_dec},

    // ZERO-PAGE,Y INDEXED ADDRESSING
    [CPU_OP_STX_ZPG_Y_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_y, .alu_op = cpu_alu_stx},
    [CPU_OP_LDX_ZPG_Y_INDEXED] = {.first_step = cpu_addr_zpg_idx_low_y, .alu_op = cpu_alu_ldx},
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
