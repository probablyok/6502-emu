#ifndef CPU_STATE_H
#define CPU_STATE_H

#include <cpu_types.h>

typedef struct {
    CpuWord pc;
    CpuByte a;
    CpuByte x;
    CpuByte y;
    CpuByte sr;
    CpuByte sp;
} Cpu;

/// @brief Zeros all CPU registers
/// @param cpu The emulated 6502 CPU
void cpu_state_zero(Cpu *cpu);

/// @brief Creates 256x256 array containing bytes of memory
/// @return Memory array
CpuByte *cpu_create_mem(void);

#endif // CPU_STATE_H