#include <cpu_state.h>
#include <stdio.h>

int main(void) {
    // Create CPU
    Cpu cpu;
    cpu_state_zero(&cpu);

    // Create memory
    CpuByte *mem = cpu_create_mem();

    return 0;
}