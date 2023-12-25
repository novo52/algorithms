#ifndef SORT_HEADER
#define SORT_HEADER

#include <stdbool.h>

// Sorts n int elements of arr in ascending or descending order
void quicksort_int(int *arr, int n, bool desc);

// A comparison function
// Return value should be:
// 	< 0 if elem1 < elem2,
// 	= 0 if elem1 == elem2,
// 	> 0 if elem1 > elem2,
typedef int ComparisonFn(void *elem1, void *elem2);

// Sorts n elements of any type via a custom comparison function
// in ascending or descending order
void quicksort(void *arr, int n, size_t sizeof_element, ComparisonFn comp, bool desc);
#endif
