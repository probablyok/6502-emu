#include <stdlib.h>
#include "util.h"

void *safe_malloc(size_t n) {
    // Adapted from: https://stackoverflow.com/a/35027099
    void *p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes\n", n);
        abort();
    }
    return p;
}

void *safe_calloc(size_t n, size_t size) {
    // Adapted from: https://stackoverflow.com/a/35027099
    void *p = calloc(n, size);
    if (p == NULL) {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes\n", n);
        abort();
    }
    return p;
}