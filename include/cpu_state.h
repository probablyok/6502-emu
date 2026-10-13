#ifndef CPU_STATE
#define CPU_STATE

#include <stdint.h>

typedef uint8_t CpuByte;
typedef uint16_t CpuWord;

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

#endif // CPU_STATE