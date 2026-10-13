#include "cpu_mem.h"
#include "cpu_macros.h"
#include "util.h"

#include <stdlib.h>

void cpu_mem_create(Cpu *cpu) {
    cpu->mem = safe_calloc(CPU_MEM_MAX_SIZE, sizeof(CpuByte));
}

void cpu_mem_free(Cpu *cpu) { free(cpu->mem); }

CpuByte cpu_mem_read(Cpu *cpu, CpuWord addr) { return cpu->mem[addr]; }