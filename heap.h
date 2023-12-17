#ifndef HEAP_HEADER
#define HEAP_HEADER

#include "vector.h"
#include "util.h"
#include <limits.h>

// Heap is a max-heap. For min-heap functionality, just use negative keys
// The heap can be used in "key-only mode" by passing in a sizeof_element of 0,
// In this case, functions returning the value of elements will treat the key as the value

typedef int HeapKey;
#define HEAPKEY_MIN INT_MIN
typedef Vec Heap;

// Returns the indices of left child, right child, and parent of i, respectively
// Indices are not guaranteed to be valid (< length) in a specifc heap instance
int heap_get_left(int i);
int heap_get_right(int i);
int heap_get_parent(int i);

// Returns the key of an element in a heap
HeapKey heap_get_key(Heap *heap, int i);

// Returns a pointer to the value of an element in a heap
// If sizeof(value) is zero for this heap, it will return a pointer to the key instead
void *heap_get_value(Heap *heap, int i);

// Requires the subtree rooted at i to be an almost-heap
// Turns it into a heap
void heap_heapify(Heap *heap, int i);

// Return a pointer to newly allocated memory containing a duplicate of the max keyed element
// Note: the pointer returned from this function needs to be freed by the user
// If sizeof(value) is zero, the key is treated as the value to be returned
void *heap_extract_max(Heap *heap);

// Set key of ith element to key
// requires new key >= old key
void heap_increase_key(Heap *heap, int i, HeapKey key);

// Insert a key, value pair into the heap
// It will be automatically positioned
void heap_insert(Heap *heap, void *value, HeapKey k);

// Alloc and free
Heap *heap_alloc(size_t sizeof_element);
void heap_free(Heap *heap);

// Create a heap from a vec
void heap_build(Vec *vec);

#endif
