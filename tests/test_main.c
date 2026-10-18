#include "cpu_state.h"
#include "safe_wrappers.h"
#include <stdio.h>
#include <string.h>

static const char TEST_DIR_PATH[] = "tests/test_json/6502/v1";
static const char JSON_EXTENSION[] = ".json";

static unsigned int test_opcode(const char *filepath) {
    printf("Running %s...\n", filepath);
    fflush(stdout);

    // Load JSON
    char *json_buf = NULL;
    if (safe_read_file(filepath, &json_buf)) {
        return 1;
    }
    cJSON *root = NULL;
    if (safe_json_parse(json_buf, &root)) {
        free(json_buf);
        return 1;
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

        // There are a lot of test cases so maybe only show where fails happen?
        // printf("Simulating %s...\n", name->valuestring);

        CpuByte opcode_under_test = cpu.mem[cpu.pc];
        // True if opcode not implemented
        if (cpu_inst_implemented(opcode_under_test)) {
            fprintf(stderr, "Opcode %02X implementation missing!\n",
                    opcode_under_test);
            cJSON_Delete(root);
            return 1; // Leave early
        }

        // Loop CPU tick
        cJSON *cycle_item = NULL;
        unsigned int tick = 0;
        unsigned int passed = 1;

        cJSON_ArrayForEach(cycle_item, cycles) {
            cpu_tick(&cpu); // Simulate CPU tick
            tick++;

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
                fprintf(stderr,
                        "[%s] Tick %d - Addr mismatch: Expected 0x%04X, "
                        "got 0x%04X\n",
                        name->valuestring, tick, valid_addr_cmp, cpu.addr_bus);
                passed = 0;
                break;
            }
            if (valid_data_cmp != cpu.data_bus) {
                fprintf(stderr,
                        "[%s] Tick %d - Data mismatch: Expected 0x%02X, got "
                        "0x%02X\n",
                        name->valuestring, tick, valid_data_cmp, cpu.data_bus);
                passed = 0;
                break;
            }
            if (valid_type_cmp != cpu.is_read) {
                fprintf(stderr,
                        "[%s] Tick %d - R/W mismatch: Expected %d, got %d\n",
                        name->valuestring, tick, valid_type_cmp, cpu.is_read);
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
                fprintf(stderr, "[%s] PC Error: Expected 0x%04X, got 0x%04X\n",
                        name->valuestring, valid_pc, cpu.pc);
                passed = 0;
            }
            if (valid_sp != cpu.sp) {
                fprintf(stderr, "[%s] SP Error: Expected 0x%02X, got 0x%02X\n",
                        name->valuestring, valid_sp, cpu.sp);
                passed = 0;
            }
            if (valid_sr != cpu.sr) {
                fprintf(stderr, "[%s] SR Error: Expected 0x%02X, got 0x%02X\n",
                        name->valuestring, valid_sr, cpu.sr);
                passed = 0;
            }
            if (valid_a != cpu.a) {
                fprintf(stderr, "[%s] A Error: Expected 0x%02X, got 0x%02X\n",
                        name->valuestring, valid_a, cpu.a);
                passed = 0;
            }
            if (valid_x != cpu.x) {
                fprintf(stderr, "[%s] X Error: Expected 0x%02X, got 0x%02X\n",
                        name->valuestring, valid_x, cpu.x);
                passed = 0;
            }
            if (valid_y != cpu.y) {
                fprintf(stderr, "[%s] Y Error: Expected 0x%02X, got 0x%02X\n",
                        name->valuestring, valid_y, cpu.y);
                passed = 0;
            }

            if (passed) {
                // Test was successful
                passed_cnt++;
            }
        }
    }

    printf("Done. %d/%d passed.\n", passed_cnt, test_case_cnt);

    // Free JSON
    cJSON_Delete(root);

    // 0 if all test cases pass, 1 otherwise
    if (passed_cnt == test_case_cnt) {
        return 0;
    }
    return 1;
}

char *get_json_fullpath(const char *filename) {
    size_t fullpath_len =
        strlen(TEST_DIR_PATH) + strlen(filename) + strlen(JSON_EXTENSION) + 3;
    char *fullpath = safe_malloc(fullpath_len);
    snprintf(fullpath, fullpath_len, "%s/%s%s", TEST_DIR_PATH, filename,
             JSON_EXTENSION);
    return fullpath;
}

int main(int argc, char **argv) {
    printf("Starting NMOS 6502 Emulator Test Suite...\n");
    printf("-----------------------------------------\n");

    unsigned int opcode_cnt = argc - 1;
    unsigned int opcode_pass = 0;

    char *fullpath;

    if (argc == 1) {
        opcode_cnt = 256;
        // Run all tests
        char hex_str[3];
        for (unsigned int i = 0; i <= 0xFF; i++) {
            snprintf(hex_str, sizeof(hex_str), "%02x", i);

            // Run test
            fullpath = get_json_fullpath(hex_str);
            opcode_pass += !(test_opcode(fullpath));
            free(fullpath);
        }
    } else {
        // Run individual tests
        for (unsigned int i = 1; i < (unsigned int)argc; i++) {
            // Run test
            fullpath = get_json_fullpath(argv[i]);
            opcode_pass += !(test_opcode(fullpath));
            free(fullpath);
        }
    }
    printf("All tests completed! %d/%d opcodes completely pass.\n", opcode_pass,
           opcode_cnt);
    return 0;
}