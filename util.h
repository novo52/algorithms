
#ifndef UTIL_HEADER
#define UTIL_HEADER

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Swap two memory regions of the same size
void memswap(void *p1, void *p2, size_t size);

// Make a copy of the data at p, returning a pointer to the new memory
// Does not alter the contents of the memory at p
void *memdup(void *p, size_t size);

void print_int_array(int *arr, int n);

#endif
