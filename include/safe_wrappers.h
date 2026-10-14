#ifndef SAFE_WRAPPERS_H
#define SAFE_WRAPPERS_H

#include "cJSON.h"
#include <stdlib.h>

/// @brief Malloc wrapper that core dumps on allocation failure
/// @param n Number of bytes to allocate
/// @return Void pointer to allocated memory
void *safe_malloc(size_t n);

/// @brief Reads the contents of a file at given path into buffer. Buffer memory
/// is allocated within function and must be freed later
/// @param filepath The string filepath to the file that will be opened
/// @param buffer Character buffer that holds contents of file
/// @return 1 if the file was unable to be opened, 0 otherwise
int safe_read_file(const char *filepath, char **buffer);

/// @brief Safely parses JSON in buffer to `cJSON` object. `cJSON` object must
/// be manually freed, and presumably, the buffer should be freed afterwards,
/// too
/// @param buffer Character buffer containing JSON to be parsed
/// @param json `cJSON` object encapsulating parsed JSON in buffer
/// @return 1 if the JSON could not be parsed (no need to free `json` in
/// this case), 0 otherwise
int safe_json_parse(const char *buffer, cJSON **json);

#endif // SAFE_WRAPPERS_H