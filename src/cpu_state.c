#include "cpu_state.h"
#include "cpu_macros.h"
#include "cpu_mem.h"
#include "util.h"
#include <stdlib.h>

/// @brief Lookup table where opcode is assigned to CPU instruction with
/// specific per-cycle steps
static CpuInstruction *INSTRUCTION_TABLE[256];

void cpu_free_mem(Cpu *cpu) { free(cpu->mem); }

void cpu_tick(Cpu *cpu) { cpu->cur_step(cpu); }

void cpu_fetch_opcode(Cpu *cpu) {
    // Read opcode from PC
    cpu->opcode = cpu_mem_read(cpu, cpu->pc++);
    // Get instruction from opcode
    CpuInstruction *instruction = INSTRUCTION_TABLE[cpu->opcode];

    // Save operation being carried out until address resolved
    cpu->pending_operation = instruction->operation;
    // Start pipeline
    cpu->cur_step = instruction->first_step;
}