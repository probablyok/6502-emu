#include "cpu_macros.h"
#include "cpu_mem.h"
#include "cpu_state.h"
#include <stdio.h>

int main(void) {
    // Create CPU and initialise memory
    Cpu cpu = {0};
    cpu_mem_create(&cpu);

    // Mock CPU state
    cpu.pc = 0x8000;
    cpu.cur_step = cpu_fetch_opcode;

    // Mock memory
    cpu.mem[0x8000] = CPU_OP_LDA_IMMEDIATE;
    cpu.mem[0x8001] = 0xF2;

    // Run for 2 cycles
    cpu_tick(&cpu);
    cpu_tick(&cpu);

    // Cleanup CPU memory
    cpu_mem_free(&cpu);
    return 0;
}
