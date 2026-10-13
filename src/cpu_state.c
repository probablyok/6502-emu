#include <cpu_macros.h>
#include <cpu_state.h>
#include <stdlib.h>
#include <util.h>

CpuByte *cpu_create_mem(void) {
    CpuByte *buf = safe_malloc(sizeof(CpuByte) * CPU_MEM_MAX_SIZE);

    // Do other memory initialisation soon
    return buf;
}

void cpu_state_zero(Cpu *cpu) {
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = 0;
    cpu->sr = 0;
    cpu->pc = 0;
}