#ifndef CPU_MEM_H
#define CPU_MEM_H
#include "cpu_state.h"

/// @brief Returns the byte stored in the specified CPU memory address. Also
/// sets the buses and flags internally in CPU as a side effect
/// @param cpu The emulated 6502 CPU whose memory is being read
/// @param addr The address of the byte being read
/// @return The byte stored in `addr`
CpuByte cpu_mem_read(Cpu *cpu, CpuWord addr);

/// @brief Writes the specified bye to the address specified in the CPU memory.
/// Also sets the buses and flags internally in CPU as a side effect
/// @param cpu The emulated 6502 CPU whose memory is being written to
/// @param addr The address of the byte being written
/// @param data The byte to be written to `addr`
void cpu_mem_write(Cpu *cpu, CpuWord addr, CpuByte data);

#endif // CPU_MEM_H