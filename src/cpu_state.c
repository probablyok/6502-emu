#include <cpu_state.h>

void cpu_state_zero(Cpu *cpu) {
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = 0;
    cpu->sr = 0;
    cpu->pc = 0;
}