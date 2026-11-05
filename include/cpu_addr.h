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
void cpu_addr_abs_x_low(Cpu* cpu);

/// @brief Read high byte for absolute X indexing
/// @brief The emulated 6502 CPU
void cpu_addr_abs_x_high(Cpu* cpu);

/// @brief Perform effective read at potentially incorrect address if page boundary was crossed. If
/// it was fine, fetches next opcode, otherwise needs to effective read on next cycle
/// @brief The emulated 6502 CPU
void cpu_addr_abs_x_read1(Cpu* cpu);

/// @brief Perform effective read and get next opcode after fixing page boundary crossing
/// @brief The emulated 6502 CPU
void cpu_addr_abs_x_read2(Cpu* cpu);

/// @brief Read low byte for absolute X indexing on write-based opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_x_write_low(Cpu* cpu);

/// @brief Read high byte for absolute X indexing on write-based opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_x_write_high(Cpu* cpu);

/// @brief Perform effective read at potentially incorrect address if page boundary was crossed.
/// For write-based opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_x_write_read(Cpu* cpu);

/// @brief Perform effective read and get next opcode after fixing page boundary crossing for write
/// based-opcodes
/// @brief The emulated 6502 CPU
void cpu_addr_abs_x_write(Cpu* cpu);

#endif
