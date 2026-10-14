#include "cpu_mem.h"
#include "cpu_macros.h"

#include <stdlib.h>

CpuByte cpu_mem_read(Cpu *cpu, CpuWord addr) {
    cpu->is_read = CPU_IS_READ;
    cpu->addr_bus = addr;
    cpu->data_bus = cpu->mem[addr];

    return cpu->data_bus;
}

void cpu_mem_write(Cpu *cpu, CpuWord addr, CpuByte data) {
    cpu->is_read = CPU_IS_WRITE;
    cpu->addr_bus = addr;
    cpu->data_bus = data;
    cpu->mem[addr] = data;
}