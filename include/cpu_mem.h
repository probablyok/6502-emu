#ifndef CPU_MEM_H
#define CPU_MEM_H
#include "cpu_state.h"

/// @brief Creates 256x256 array containing bytes of memory for CPU
/// @param cpu The emulated 6502 CPU
void cpu_mem_create(Cpu *cpu);

/// @brief Frees the 256x256 array for memory in CPU
/// @param cpu The emulated 6502 CPU
void cpu_mem_free(Cpu *cpu);

/// @brief Returns the byte stored in the specified CPU memory address
/// @param cpu The emulated 6502 CPU whose memory is being read
/// @param addr The address of the byte being read
/// @return The byte stored in `addr`
CpuByte cpu_mem_read(Cpu *cpu, CpuWord addr);
#endif // CPU_MEM_H