#include "cpu_mem.h"
#include "cpu_macros.h"
#include "util.h"

/// @brief Creates 256x256 array containing bytes of memory for CPU
/// @param cpu The emulated 6502 CPU
static void cpu_mem_create(Cpu *cpu);

static void cpu_mem_create(Cpu *cpu) {
    cpu->mem = safe_calloc(CPU_MEM_MAX_SIZE, sizeof(CpuByte));
}

CpuByte cpu_mem_read(Cpu *cpu, CpuWord addr) { return cpu->mem[addr]; }