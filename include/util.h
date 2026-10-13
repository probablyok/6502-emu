#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

/// @brief Wrapper for `malloc` that crashes with error message
/// @param n The number of bytes to allocate
/// @return The pointer to memory allocated
void *safe_malloc(size_t n);

/// @brief Wrapper for `calloc` that crashes with error message
/// @param n The number of elements to allocate
/// @param size Number of bytes per element to allocate
/// @return The pointer to memory allocated
void *safe_calloc(size_t n, size_t size);

#endif // UTIL_H