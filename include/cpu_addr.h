#ifndef CPU_ADDR_
#define CPU_ADDR_

#include "cpu_state.h"

/// @brief Reads the current opcode in the program counter before incrementing
/// it. Then performs decode logic to get instruction steps
/// @param cpu The emulated 6502 CPU
void cpu_fetch_opcode(Cpu* cpu);

/// @brief Handle immediate addressing and run pending ALU
/// @param cpu The emulated 6502 CPU
void cpu_addr_immediate(Cpu* cpu);

/// @brief Handle implied addressing and run pending ALU
/// @param cpu The emulated 6502 CPU
void cpu_addr_implied(Cpu* cpu);

/// @brief Read the low address byte for abs jmp
/// @param The emulated 6052 CPU
void cpu_addr_absolute_jmp_low(Cpu* cpu);

/// @brief Read the high address byte for abs jmp and change PC
/// @param The emulated 6502 CPU
void cpu_addr_absolute_jmp_high(Cpu* cpu);

/// @brief Read the low address byte for indirect jmp
/// @param The emulated 6052 CPU
void cpu_addr_indirect_jmp_low(Cpu* cpu);

/// @brief Read the high address byte for indirect jmp
/// @param The emulated 6502 CPU
void cpu_addr_indirect_jmp_high(Cpu* cpu);

/// @brief Set PCL from effective address read
/// @param The emulated 6502 CPU
void cpu_addr_indirect_jmp_read1(Cpu* cpu);

/// @brief Set PCH from effective address read incremented by 1 with page wrap
/// @param The emulated 6502 CPU
void cpu_addr_indirect_jmp_read2(Cpu* cpu);

/// @brief Read the low address byte from the opcode
/// @param The emulated 6052 CPU
void cpu_addr_absolute_low(Cpu* cpu);

/// @brief Read the high address byte from the opcode
/// @param The emulated 6502 CPU
void cpu_addr_absolute_high(Cpu* cpu);

/// @brief Using the saved 16-bit word from address opcode reads, run pending ALU instruction
/// @brief The emulated 6502 CPU
void cpu_addr_aboslute(Cpu* cpu);

/// @brief Read the high address byte from the opcode for RMW abs instructions
/// @param The emulated 6502 CPU
void cpu_addr_absolute_rmw_low(Cpu* cpu);

/// @brief Read the high address byte from the opcode for RMW abs instructions
/// @param The emulated 6502 CPU
void cpu_addr_absolute_rmw_high(Cpu* cpu);

/// @brief Using the saved 16-bit word from address opcode reads, run pending ALU instruction for
/// RMW abs instructions
/// @brief The emulated 6502 CPU
void cpu_addr_absolute_rmw_read(Cpu* cpu);

/// @brief Dummy write back to effective address
/// @brief The emulated 6502 CPU
void cpu_addr_absolute_rmw_write(Cpu* cpu);

/// @brief Write operated value to effective address after doing ALU operation
/// @brief The emulated 6502 CPU
void cpu_addr_absolute_rmw_write2(Cpu* cpu);

/// @brief Read the low address byte from the opcode, assuming high byte is zero-page
/// @param The emulated 6052 CPU
void cpu_addr_zpg_low(Cpu* cpu);

/// @brief Write operated value to effective address after doing ALU operation for zero-page
/// addressing
/// @brief The emulated 6502 CPU
void cpu_addr_zpg(Cpu* cpu);

/// @brief Read the high address byte from the opcode for RMW zpg instructions
/// @param The emulated 6502 CPU
void cpu_addr_zpg_rmw_low(Cpu* cpu);

/// @brief Using the saved 16-bit word from address opcode reads, run pending ALU instruction for
/// RMW zpg instructions
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_rmw_read(Cpu* cpu);

/// @brief Dummy write back to effective address for RMW zpg instructions
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_rmw_write(Cpu* cpu);

/// @brief Write operated value to effective address after doing ALU operation for RMW zpg
/// instructions
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_rmw_write2(Cpu* cpu);

/// @brief Read low byte for absolute X indexing
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_low_x(Cpu* cpu);

/// @brief Read low byte for absolute Y indexing
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_low_y(Cpu* cpu);

/// @brief Read high byte for absolute indexing
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_high(Cpu* cpu);

/// @brief Perform effective read at potentially incorrect address if page boundary was crossed. If
/// it was fine, fetches next opcode, otherwise needs to effective read on next cycle
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_read1(Cpu* cpu);

/// @brief Perform effective read and get next opcode after fixing page boundary crossing
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_read2(Cpu* cpu);

/// @brief Read low byte for absolute X indexing on write-based opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_write_low_x(Cpu* cpu);

/// @brief Read low byte for absolute Y indexing on write-based opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_write_low_y(Cpu* cpu);

/// @brief Read high byte for absoluteindexing on write-based opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_write_high(Cpu* cpu);

/// @brief Perform effective read at potentially incorrect address if page boundary was crossed.
/// For write-based opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_write_read(Cpu* cpu);

/// @brief Perform effective read and get next opcode after fixing page boundary crossing for write
/// based-opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_write(Cpu* cpu);

/// @brief Read low byte for absolute X indexing on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_rmw_low_x(Cpu* cpu);

/// @brief Read low byte for absolute Y indexing on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_rmw_low_y(Cpu* cpu);

/// @brief Read high byte for absolute Y indexing on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_rmw_high(Cpu* cpu);

/// @brief Perform effective read at potentially incorrect address if page boundary was crossed and
/// fix for next cylce
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_rmw_read1(Cpu* cpu);

/// @brief Perform read at fixed effective address
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_rmw_read2(Cpu* cpu);

/// @brief Dummy write of non-operated value
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_rmw_write1(Cpu* cpu);

/// @brief Effective write of operated value
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_rmw_write2(Cpu* cpu);

/// @brief Read low byte for zpg indexing using X reg
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_low_x(Cpu* cpu);

/// @brief Read low byte for zpg indexing using Y reg
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_low_y(Cpu* cpu);

/// @brief Dummy read of address for zpg indexing
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_read1(Cpu* cpu);

/// @brief Effective read of address with index added for zpg indexing
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_read2(Cpu* cpu);

/// @brief Read low byte for zpg indexing using X reg on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_rmw_low_x(Cpu* cpu);

/// @brief Read low byte for zpg indexing using Y reg on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_rmw_low_y(Cpu* cpu);

/// @brief Dummy read of address for zpg indexing on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_rmw_read1(Cpu* cpu);

/// @brief Effective read of address with index added for zpg indexing on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_rmw_read2(Cpu* cpu);

/// @brief Dummy write of data for zpg indexing on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_rmw_write1(Cpu* cpu);

/// @brief Effective write of operated data for zpg indexing on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_rmw_write2(Cpu* cpu);

void cpu_addr_pre_idx_indirect_low(Cpu* cpu);
void cpu_addr_pre_idx_indirect_read_ptr(Cpu* cpu);
void cpu_addr_pre_idx_indirect_fetch_low(Cpu* cpu);
void cpu_addr_pre_idx_indirect_fetch_high(Cpu* cpu);
void cpu_addr_pre_idx_indirect_read(Cpu* cpu);

void cpu_addr_post_idx_indirect_low(Cpu* cpu);
void cpu_addr_post_idx_indirect_fetch_low(Cpu* cpu);
void cpu_addr_post_idx_indirect_fetch_high(Cpu* cpu);
void cpu_addr_post_idx_indirect_read1(Cpu* cpu);
void cpu_addr_post_idx_indirect_read2(Cpu* cpu);

void cpu_addr_post_idx_indirect_write_low(Cpu* cpu);
void cpu_addr_post_idx_indirect_write_fetch_low(Cpu* cpu);
void cpu_addr_post_idx_indirect_write_fetch_high(Cpu* cpu);
void cpu_addr_post_idx_indirect_write_read(Cpu* cpu);
void cpu_addr_post_idx_indirect_write_write(Cpu* cpu);

void cpu_addr_relative_fetch_offset(Cpu* cpu);
void cpu_addr_relative_dummy_read(Cpu* cpu);
void cpu_addr_relative_invalid_read(Cpu* cpu);

void cpu_addr_brk_dummy_read(Cpu* cpu);
void cpu_addr_brk_push1(Cpu* cpu);
void cpu_addr_brk_push2(Cpu* cpu);
void cpu_addr_brk_push3(Cpu* cpu);
void cpu_addr_brk_vec_low_fetch(Cpu* cpu);
void cpu_addr_brk_vec_high_fetch(Cpu* cpu);

void cpu_addr_rti_dummy_read(Cpu* cpu);
void cpu_addr_rti_inc_sp(Cpu* cpu);
void cpu_addr_rti_pull1(Cpu* cpu);
void cpu_addr_rti_pull2(Cpu* cpu);
void cpu_addr_rti_pull3(Cpu* cpu);

void cpu_addr_jsr_fetch_low(Cpu* cpu);
void cpu_addr_jsr_dummy(Cpu* cpu);
void cpu_addr_jsr_push1(Cpu* cpu);
void cpu_addr_jsr_push2(Cpu* cpu);
void cpu_addr_jsr_fetch_high(Cpu* cpu);

void cpu_addr_rts_dummy1(Cpu* cpu);
void cpu_addr_rts_dummy2(Cpu* cpu);
void cpu_addr_rts_pull1(Cpu* cpu);
void cpu_addr_rts_pull2(Cpu* cpu);
void cpu_addr_rts_inc(Cpu* cpu);

#endif
