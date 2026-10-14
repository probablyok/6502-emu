#include "cpu_state.h"
#include "safe_wrappers.h"
#include <stdio.h>
#include <string.h>

static void test_opcode(const char *filepath) {
    printf("Loading %s...\n", filepath);

    // Load JSON
    char *json_buf = NULL;
    if (safe_read_file(filepath, &json_buf)) {
        return;
    }
    cJSON *root = NULL;
    if (safe_json_parse(json_buf, &root)) {
        free(json_buf);
        return;
    }
    free(json_buf);

    // For each JSON test case:
    cJSON *test_case = NULL;
    unsigned int test_case_cnt = 0;
    unsigned int passed_cnt = 0;

    cJSON_ArrayForEach(test_case, root) {
        test_case_cnt++;
        cJSON *name = cJSON_GetObjectItemCaseSensitive(test_case, "name");
        cJSON *initial = cJSON_GetObjectItemCaseSensitive(test_case, "initial");
        cJSON *final = cJSON_GetObjectItemCaseSensitive(test_case, "final");
        cJSON *cycles = cJSON_GetObjectItemCaseSensitive(test_case, "cycles");

        // Init clean CPU state
        Cpu cpu = {0};
        memset(cpu.mem, 0, sizeof(cpu.mem));
        cpu.cur_step = cpu_fetch_opcode;

        // Setup registers
        cpu.pc =
            (CpuWord)cJSON_GetObjectItemCaseSensitive(initial, "pc")->valueint;
        cpu.sp =
            (CpuByte)cJSON_GetObjectItemCaseSensitive(initial, "s")->valueint;
        cpu.sr =
            (CpuByte)cJSON_GetObjectItemCaseSensitive(initial, "p")->valueint;
        cpu.a =
            (CpuByte)cJSON_GetObjectItemCaseSensitive(initial, "a")->valueint;
        cpu.x =
            (CpuByte)cJSON_GetObjectItemCaseSensitive(initial, "x")->valueint;
        cpu.y =
            (CpuByte)cJSON_GetObjectItemCaseSensitive(initial, "y")->valueint;

        // Setup memory
        cJSON *ram = cJSON_GetObjectItemCaseSensitive(initial, "ram");
        cJSON *ram_item = NULL;
        cJSON_ArrayForEach(ram_item, ram) {
            CpuWord addr = (CpuWord)cJSON_GetArrayItem(ram_item, 0)->valueint;
            CpuByte data = (CpuByte)cJSON_GetArrayItem(ram_item, 1)->valueint;
            cpu.mem[addr] = data;
        }

        printf("Simulating %s...\n", name->valuestring);

        // Loop CPU tick
        cJSON *cycle_item = NULL;
        unsigned int tick = 0;
        unsigned int passed = 1;
        cJSON_ArrayForEach(cycle_item, cycles) {
            cpu_tick(&cpu); // Simulate CPU tick
            tick++;
            printf("Simulated tick %d...\n", tick);

            // Verify cycle readings
            CpuWord valid_addr_cmp =
                (CpuWord)cJSON_GetArrayItem(cycle_item, 0)->valueint;
            CpuByte valid_data_cmp =
                (CpuByte)cJSON_GetArrayItem(cycle_item, 1)->valueint;
            char *valid_type = cJSON_GetArrayItem(cycle_item, 2)->valuestring;
            CpuByte valid_type_cmp =
                strcmp(valid_type, "read") ? CPU_IS_WRITE : CPU_IS_READ;

            // Compare
            if (valid_addr_cmp != cpu.addr_bus) {
                fprintf(
                    stderr,
                    "Error: Expected address %d, got %d. Aborting test case!\n",
                    valid_addr_cmp, cpu.addr_bus);
                passed = 0;
                break;
            }
            if (valid_data_cmp != cpu.data_bus) {
                fprintf(
                    stderr,
                    "Error: Expected data %d, got %d. Aborting test case!\n",
                    valid_data_cmp, cpu.data_bus);
                passed = 0;
                break;
            }
            if (valid_type_cmp != cpu.is_read) {
                fprintf(stderr,
                        "Error: Expected R/W %d, got %d. Aborting test case!\n",
                        valid_type_cmp, cpu.is_read);
                passed = 0;
                break;
            }
        }

        // Don't run register and memory test if cycle-by-cycle verification
        // failed
        if (passed) {
            // Verify registers and memory
            CpuWord valid_pc =
                (CpuWord)cJSON_GetObjectItemCaseSensitive(final, "pc")
                    ->valueint;
            CpuByte valid_sp =
                (CpuByte)cJSON_GetObjectItemCaseSensitive(final, "s")->valueint;
            CpuByte valid_sr =
                (CpuByte)cJSON_GetObjectItemCaseSensitive(final, "p")->valueint;
            CpuByte valid_a =
                (CpuByte)cJSON_GetObjectItemCaseSensitive(final, "a")->valueint;
            CpuByte valid_x =
                (CpuByte)cJSON_GetObjectItemCaseSensitive(final, "x")->valueint;
            CpuByte valid_y =
                (CpuByte)cJSON_GetObjectItemCaseSensitive(final, "y")->valueint;

            if (valid_pc != cpu.pc) {
                fprintf(stderr,
                        "Error: Expected PC %d, got %d. Aborting test case!\n",
                        valid_pc, cpu.pc);
                break;
            }
            if (valid_sp != cpu.sp) {
                fprintf(stderr,
                        "Error: Expected SP %d, got %d. Aborting test case!\n",
                        valid_sp, cpu.sp);
                break;
            }
            if (valid_sr != cpu.sr) {
                fprintf(stderr,
                        "Error: Expected SR %d, got %d. Aborting test case!\n",
                        valid_sr, cpu.sr);
                break;
            }
            if (valid_a != cpu.a) {
                fprintf(stderr,
                        "Error: Expected A %d, got %d. Aborting test case!\n",
                        valid_a, cpu.a);
                break;
            }
            if (valid_x != cpu.x) {
                fprintf(stderr,
                        "Error: Expected X %d, got %d. Aborting test case!\n",
                        valid_x, cpu.x);
                break;
            }
            if (valid_y != cpu.y) {
                fprintf(stderr,
                        "Error: Expected Y %d, got %d. Aborting test case!\n",
                        valid_y, cpu.y);
                break;
            }

            // Test was successful
            passed_cnt++;
            printf("Test case successful!\n");
        }
    }

    printf("%s outcome: %d/%d\n", filepath, passed_cnt, test_case_cnt);

    // Free JSON
    cJSON_Delete(root);
}

int main(void) {
    printf("Starting 6502 Emulator Test Suite...\n");
    printf("------------------------------------\n");

    test_opcode("tests/a9.json");

    printf("All tests completed!\n");
    return 0;
}