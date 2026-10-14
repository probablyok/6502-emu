#include "cpu_state.h"
#include "safe_wrappers.h"
#include <stdio.h>

static void test_opcode(const char *filepath) {
    printf("Loading %s...\n", filepath);

    // Load JSON
    char *jsonBuffer = NULL;
    if (safe_read_file(filepath, jsonBuffer)) {
        return;
    }
    cJSON *root = NULL;
    if (safe_json_parse(jsonBuffer, root)) {
        free(jsonBuffer);
        return;
    }

    // For each JSON test case:

    // Init clean CPU state
    // Load intiial reigsters and memory array
    // Loop cpu_tick
    // Verify memory bus read/writes match expected cycle array
    // Verify final CPU registers match JSON final state

    // Free JSON
    free(jsonBuffer);
}

int main(void) {
    printf("Starting 6502 Emulator Test Suite...\n");
    printf("------------------------------------\n");

    printf("All tests completed!\n");
    return 0;
}