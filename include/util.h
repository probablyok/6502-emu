#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

/// @brief Wrapper for `malloc` that crashes with error message
/// @param n The number of bytes to allocate
/// @return The pointer to memory allocated
void *safe_malloc(size_t n);

#endif // UTIL_H