#include "cpu_state.h"
#include "cpu_macros.h"
#include "util.h"
#include <stdlib.h>

/// @brief Creates 256x256 array containing bytes of memory for CPU
/// @param cpu The emulated 6502 CPU
static void cpu_create_mem(Cpu *cpu);

/// @brief Lookup table where opcode is assigned to CPU instruction with
/// specific per-cycle steps
static void (*INSTRUCTION_TABLE[256])(CpuInstruction *);

static void cpu_create_mem(Cpu *cpu) {
    cpu->mem = safe_calloc(CPU_MEM_MAX_SIZE, sizeof(CpuByte));
}

void cpu_free_mem(Cpu *cpu) { free(cpu->mem); }