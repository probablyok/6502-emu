#ifndef CPU_ADDR_H
#define CPU_ADDR_H

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

/// @brief Read the low address byte for indirect jmp
/// @param The emulated 6052 CPU
void cpu_addr_indirect_jmp_low(Cpu* cpu);

/// @brief Read the low address byte for abs jmp
/// @param The emulated 6052 CPU
void cpu_addr_absolute_jmp_low(Cpu* cpu);

/// @brief Read the low address byte from the opcode
/// @param The emulated 6052 CPU
void cpu_addr_absolute_low(Cpu* cpu);

/// @brief Read the high address byte from the opcode for RMW abs instructions
/// @param The emulated 6502 CPU
void cpu_addr_absolute_rmw_low(Cpu* cpu);

/// @brief Read the low address byte from the opcode, assuming high byte is zero-page
/// @param The emulated 6052 CPU
void cpu_addr_zpg_low(Cpu* cpu);

/// @brief Read the high address byte from the opcode for RMW zpg instructions
/// @param The emulated 6502 CPU
void cpu_addr_zpg_rmw_low(Cpu* cpu);

/// @brief Read low byte for absolute X indexing
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_low_x(Cpu* cpu);
/// @brief Read low byte for absolute Y indexing
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_low_y(Cpu* cpu);

/// @brief Read low byte for absolute X indexing on write-based opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_write_low_x(Cpu* cpu);
/// @brief Read low byte for absolute Y indexing on write-based opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_write_low_y(Cpu* cpu);

/// @brief Read low byte for absolute X indexing on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_rmw_low_x(Cpu* cpu);
/// @brief Read low byte for absolute Y indexing on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_idx_rmw_low_y(Cpu* cpu);

/// @brief Read low byte for zpg indexing using X reg
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_low_x(Cpu* cpu);
/// @brief Read low byte for zpg indexing using Y reg
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_low_y(Cpu* cpu);

/// @brief Read low byte for zpg indexing using X reg on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_rmw_low_x(Cpu* cpu);
/// @brief Read low byte for zpg indexing using Y reg on RMW opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_zpg_idx_rmw_low_y(Cpu* cpu);

/// @brief Read low byte for pre-indexed indirect opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_pre_idx_indirect_low(Cpu* cpu);

/// @brief Read low byte for post-indexed indirect opcodes read instructions
/// @brief The emulated 6502 CPU
void cpu_addr_post_idx_indirect_low(Cpu* cpu);

/// @brief Read low byte for post-indexed indirect opcodes write instructions
/// @brief The emulated 6502 CPU
void cpu_addr_post_idx_indirect_write_low(Cpu* cpu);

/// @brief Entrypoint for relative addressing opcodes (conditional opcodes)
/// @brief The emulated 6502 CPU
void cpu_addr_relative_fetch_offset(Cpu* cpu);

/// @brief Entrypoint for BRK opcode
/// @brief The emulated 6502 CPU
void cpu_addr_brk_dummy_read(Cpu* cpu);

/// @brief Entrypoint for RTI opcode
/// @brief The emulated 6502 CPU
void cpu_addr_rti_dummy_read(Cpu* cpu);

/// @brief Entrypoint for JSR opcode
/// @brief The emulated 6502 CPU
void cpu_addr_jsr_fetch_low(Cpu* cpu);

/// @brief Entrypoint for RTS opcode
/// @brief The emulated 6502 CPU
void cpu_addr_rts_dummy1(Cpu* cpu);

#endif // CPU_ADDR_H
