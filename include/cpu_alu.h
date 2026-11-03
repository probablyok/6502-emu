#ifndef CPU_ALU_
#define CPU_ALU_

#include "cpu_state.h"

/// @brief LDA Instruction ALU logic: Load accumulator with memory in data bus
/// @param cpu The emulated 6502 CPU
void cpu_alu_lda(Cpu* cpu);

/// @brief LDX Instruction ALU logic: Load X with memory in data bus
/// @param cpu The emulated 6502 CPU
void cpu_alu_ldx(Cpu* cpu);

/// @brief LDY Instruction ALU logic: Load Y with memory in data bus
/// @param cpu The emulated 6502 CPU
void cpu_alu_ldy(Cpu* cpu);

/// @brief AND Instruction ALU logic: AND data bus with accumulator and store
/// back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_and(Cpu* cpu);

/// @brief ORA Instruction ALU logic: OR data bus with accumulator and store
/// back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_ora(Cpu* cpu);

/// @brief EOR Instruction ALU logic: XOR data bus with accumulator and store
/// back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_eor(Cpu* cpu);

/// @brief ADC Instruction ALU logic: add data bus with accumulator with carry
/// and store back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_adc(Cpu* cpu);

/// @brief CMP Instruction ALU logic: CMP (subtract) data bus with accumulator
/// and update flags without storing result back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_cmp(Cpu* cpu);

/// @brief CPY Instruction ALU logic: CMP (subtract) data bus with Y
/// and update flags without storing result back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_cpy(Cpu* cpu);

/// @brief CPX Instruction ALU logic: CMP (subtract) data bus with X
/// and update flags without storing result back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_cpx(Cpu* cpu);

/// @brief SBC Instruction ALU logic: subtract data bus from accumulator with
/// carry and store back in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_sbc(Cpu* cpu);

/// @brief SED Instruction ALU logic: Set the decimal flag in the status
/// register
/// @param cpu The emulated 6502 CPU
void cpu_alu_sed(Cpu* cpu);

/// @brief CLD Instruction ALU logic: Unet the decimal flag in the status
/// register
/// @param cpu The emulated 6502 CPU
void cpu_alu_cld(Cpu* cpu);

/// @brief NOP Instruction ALU logic: Do nothing
/// @param cpu The emulated 6502 CPU
void cpu_alu_nop(Cpu* cpu);

/// @brief CLC Instruction ALU logic: Unset carry flag in status register
/// @param cpu The emulated 6502 CPU
void cpu_alu_clc(Cpu* cpu);

/// @brief SEC Instruction ALU logic: Set carry flag in status register
/// @param cpu The emulated 6502 CPU
void cpu_alu_sec(Cpu* cpu);

/// @brief DEY Instruction ALU logic: Decrement Y by 1
/// @param cpu The emulated 6502 CPU
void cpu_alu_dey(Cpu* cpu);

/// @brief DEX Instruction ALU logic: Decrement X by 1
/// @param cpu The emulated 6502 CPU
void cpu_alu_dex(Cpu* cpu);

/// @brief TXA Instruction ALU logic: Copy X into A
/// @param cpu The emulated 6502 CPU
void cpu_alu_txa(Cpu* cpu);

/// @brief TYA Instruction ALU logic: Copy Y into A
/// @param cpu The emulated 6502 CPU
void cpu_alu_tya(Cpu* cpu);

/// @brief TAX Instruction ALU logic: Copy A into X
/// @param cpu The emulated 6502 CPU
void cpu_alu_tax(Cpu* cpu);

/// @brief TAY Instruction ALU logic: Copy A into Y
/// @param cpu The emulated 6502 CPU
void cpu_alu_tay(Cpu* cpu);

/// @brief CLV Instruction ALU logic: Unset the overflow status register flag
/// @param cpu The emulated 6502 CPU
void cpu_alu_clv(Cpu* cpu);

/// @brief INX Instruction ALU logic: Increment the value in the X register
/// @param cpu The emulated 6502 CPU
void cpu_alu_inx(Cpu* cpu);

/// @brief INY Instruction ALU logic: Increment the value in the Y register
/// @param cpu The emulated 6502 CPU
void cpu_alu_iny(Cpu* cpu);

/// @brief PHP/PHA Instruction cycle 2: Does dummy read on next instruction byte
/// and does nothing
/// @param cpu The emulated 6502 CPU
void cpu_wait_php_pha(Cpu* cpu);

/// @brief Final step in instruction with many cycles: Performs the ALU
/// operation and prepares to read next instruction byte as opcode
/// @param cpu The emulated 6502 CPU
void cpu_wait_do_alu(Cpu* cpu);

/// @brief PHP Instruction ALU logic: Push the status register onto the stack
/// @param cpu The emulated 6502 CPU
void cpu_alu_php(Cpu* cpu);

/// @brief PHA Instruction ALU logic: Push the accumulator onto the stack
/// @param cpu The emulated 6502 CPU
void cpu_alu_pha(Cpu* cpu);

/// @brief PLP Instruction cycle 2: Does dummy read on next instruction byte and
/// does nothing
/// @param cpu The emulated 6502 CPU
void cpu_wait1_plp_pla(Cpu* cpu);

/// @brief PLP Instruction cycle 3: Does dummy read on current stack pointer and
/// does nothing
/// @param cpu The emulated 6502 CPU
void cpu_wait2_plp_pla(Cpu* cpu);

/// @brief PLP Instruction ALU logic: Pull new status register value from stack
/// @param cpu The emulated 6502 CPU
void cpu_alu_plp(Cpu* cpu);

/// @brief PLA Instruction ALU logic: Pull new accumulator value from stack
/// @param cpu The emulated 6502 CPU
void cpu_alu_pla(Cpu* cpu);

/// @brief TXS Instruction ALU logic: Set stack pointer to X register
/// @param cpu The emulated 6502 CPU
void cpu_alu_txs(Cpu* cpu);

/// @brief TSX Instruction ALU logic: Set X register to stack pointer
/// @param cpu The emulated 6502 CPU
void cpu_alu_tsx(Cpu* cpu);

/// @brief CLI Instruction ALU logic: Set interrupt disable flag in status
/// register
/// @param cpu The emulated 6502 CPU
void cpu_alu_cli(Cpu* cpu);

/// @brief SEI Instruction ALU logic: Unset interrupt disable flag in status
/// register
/// @param cpu The emulated 6502 CPU
void cpu_alu_sei(Cpu* cpu);

/// @brief ASL Instruction ALU logic: Shift left one bit in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_asl(Cpu* cpu);

/// @brief ROL Instruction ALU logic: Rotate one bit left in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_rol(Cpu* cpu);

/// @brief LSR Instruction ALU logic: Shift right one bit in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_lsr(Cpu* cpu);

/// @brief ROR Instruction ALU logic: Rotate one bit right in accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_ror(Cpu* cpu);

/// @brief ROR Instruction ALU logic: Rotate one bit right in memory
/// @param cpu The emulated 6502 CPU
void cpu_alu_asl_mem(Cpu* cpu);

/// @brief ROL Instruction ALU logic: Rotate one bit left in memory
/// @param cpu The emulated 6502 CPU
void cpu_alu_rol_mem(Cpu* cpu);

/// @brief LSR Instruction ALU logic: Shift right one bit in memory
/// @param cpu The emulated 6502 CPU
void cpu_alu_lsr_mem(Cpu* cpu);

/// @brief ROR Instruction ALU logic: Rotate one bit right in memory
/// @param cpu The emulated 6502 CPU
void cpu_alu_ror_mem(Cpu* cpu);

/// @brief BIT Instruction ALU logic: Tests bits in memory with accumulator
/// @param cpu The emulated 6502 CPU
void cpu_alu_bit(Cpu* cpu);

/// @brief INC Instruction ALU logic: Increment value in memory by one
/// @param cpu The emulated 6502 CPU
void cpu_alu_inc(Cpu* cpu);

/// @brief DEC Instruction ALU logic: Decrement value in memory by one
/// @param cpu The emulated 6502 CPU
void cpu_alu_dec(Cpu* cpu);

/// @brief STY Instruction ALU logic: Store register Y in memory
/// @param cpu The emulated 6502 CPU
void cpu_alu_sty(Cpu* cpu);

/// @brief STA Instruction ALU logic: Store register A in memory
/// @param cpu The emulated 6502 CPU
void cpu_alu_sta(Cpu* cpu);

/// @brief STX Instruction ALU logic: Store register X in memory
/// @param cpu The emulated 6502 CPU
void cpu_alu_stx(Cpu* cpu);
#endif
