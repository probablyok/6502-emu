#include "cpu_mem.h"
#include "cpu_macros.h"

#include <stdlib.h>

CpuByte cpu_mem_read(Cpu *cpu, CpuWord addr) { return cpu->mem[addr]; }

void cpu_mem_write(Cpu *cpu, CpuWord addr, CpuByte data) {
    cpu->mem[addr] = data;
}