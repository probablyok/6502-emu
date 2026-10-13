#ifndef CPU_MEM_H
#define CPU_MEM_H
#include "cpu_state.h"

/// @brief Returns the byte stored in the specified CPU memory address
/// @param cpu The emulated 6502 CPU whose memory is being read
/// @param addr The address of the byte being read
/// @return The byte stored in `addr`
CpuByte cpu_mem_read(Cpu *cpu, CpuWord addr);
#endif // CPU_MEM_H