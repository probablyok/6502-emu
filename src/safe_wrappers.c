#include "safe_wrappers.h"
#include <stdio.h>

void *safe_malloc(size_t n) {
    void *p = (void *)malloc(n);
    if (p == NULL) {
        fprintf(stderr, "Error: Malloc call failed. Aborting...");
        abort();
    }
    return p;
}

int safe_read_file(const char *filepath, char *buffer) {
    // Open file pointer
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open \"%s\".\n", filepath);
        return 1;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Read file into buffer
    buffer = (char *)safe_malloc(fileSize + 1);
    fread(buffer, 1, fileSize, fp);
    buffer[fileSize] = '\0';

    // Close file
    fclose(fp);
    return 0;
}

int safe_json_parse(const char *buffer, cJSON *json) {
    json = cJSON_Parse(buffer);

    if (json == NULL) {
        const char *json_err = cJSON_GetErrorPtr();
        if (json_err != NULL) {
            fprintf(stderr, "Error: JSON parse failed before: %s\n", json_err);
        }
        cJSON_Delete(json);
        return 1;
    }

    return 0;
}