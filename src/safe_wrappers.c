#include "safe_wrappers.h"
#include "cpu_macros.h"
#include <stdio.h>

void* safe_malloc(size_t n) {
    void* p = (void*)malloc(n);
    if (p == NULL) {
        fprintf(stderr, "Error: Malloc call failed. Aborting...");
        abort();
    }
    return p;
}

int safe_read_file_text(const char* filepath, char** buffer) {
    // Open file pointer
    FILE* fp = fopen(filepath, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open \"%s\".\n", filepath);
        return 1;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Read file into buffer
    *buffer = (char*)safe_malloc(file_size + 1);
    size_t bytes_read = fread(*buffer, 1, file_size, fp);

    if (bytes_read != (size_t)file_size) {
        fprintf(stderr, "Error: Failed to read entire file \"%s\".\n", filepath);
        free(*buffer);
        fclose(fp);
        return 1;
    }

    (*buffer)[file_size] = '\0';
    // Close file
    fclose(fp);
    return 0;
}

int safe_read_file_bin(const char* filepath, Cpu* cpu, long mem_offset) {
    // Is mem_offset valid
    if (mem_offset < 0 || mem_offset >= CPU_MEM_MAX_SIZE) {
        fprintf(stderr, "Error: Memory offset (0x%04X) is out of bounds.\n",
                (unsigned int)mem_offset);
        return 1;
    }

    // Open file pointer
    FILE* fp = fopen(filepath, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open \"%s\".\n", filepath);
        return 1;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Check file contents fit in memory
    if (file_size <= 0 || (mem_offset + file_size) > CPU_MEM_MAX_SIZE) {
        fprintf(stderr,
                "Error: Binary size (%ld bytes) exceeds memory limits when offset at 0x%04X.\n",
                file_size, (unsigned int)mem_offset);
        fclose(fp);
        return 1;
    }
    // Put file contents in memory
    size_t bytes_read = fread(&cpu->mem[mem_offset], 1, file_size, fp);

    // Check the binary was read correctly
    if (bytes_read != (size_t)file_size) {
        fprintf(stderr, "Error: Failed to read entire binary.\n");
        return 1;
    }

    // Close file
    fclose(fp);
    return 0;
}

int safe_json_parse(const char* buffer, cJSON** json) {
    *json = cJSON_Parse(buffer);

    if (*json == NULL) {
        const char* json_err = cJSON_GetErrorPtr();
        if (json_err != NULL) {
            fprintf(stderr, "Error: JSON parse failed before: %s\n", json_err);
        }
        cJSON_Delete(*json);
        return 1;
    }

    return 0;
}
