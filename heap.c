#include "heap.h"

#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "util.h"

int heap_get_left(int j) 	{ return (j+1)*2-1; }
int heap_get_right(int j) 	{ return (j+1)*2; }
int heap_get_parent(int j) 	{ return (j+1)/2-1; }

// Returns first 4 bytes of the element at i (which is its key as an int)
HeapKey heap_get_key(Heap *heap, int i) {
	return *(HeapKey *)vec_get(heap, i);
}

void *heap_get_value(Heap *heap, int i) {
	if (heap->sizeof_element == sizeof(HeapKey)) {
		// Value has size zero, return the key instead
		return vec_get(heap, i);
	} else {
		return (void *)((HeapKey *)vec_get(heap, i)+1);
	}
}

// Requires the subtree rooted at i to be an almost-heap
// Turns it into a heap
void heap_heapify(Heap *heap, int i) {
	int left = heap_get_left(i);
	int right = heap_get_right(i);
	int largest = i;

	// largest = max (by key) of existing children
	if (left < heap->length && heap_get_key(heap, left) > heap_get_key(heap, largest))
		largest = left;
	if (right < heap->length && heap_get_key(heap, right) > heap_get_key(heap, largest))
		largest = right;

	//printf("i = %d (v %d), left = %d (v %d), right = %d (v %d), largest = %d (v %d)\n", i, heap->root[i], left, heap->root[left], right, heap->root[right], largest, heap->root[largest]);

	// if we have a larger child, swap and fix the subtree
	if (largest != i) {
		memswap(vec_get(heap, i), vec_get(heap, largest), heap->sizeof_element);
		heap_heapify(heap, largest);
	}
}

void *heap_extract_max(Heap *heap) {

	// Copy just the value of the max element (not the key)
	void *max;
	if (heap->sizeof_element == sizeof(HeapKey)) {
		max = memdup(vec_get(heap, 0), sizeof(HeapKey));
	} else {
		max = memdup(heap_get_value(heap, 0), heap->sizeof_element-sizeof(HeapKey));
	}

	// Overwrite the max element with the last element then shrink the original out
	memcpy(vec_get(heap, 0), vec_pop(heap), heap->sizeof_element);
	if (heap->length > 0) heap_heapify(heap, 0);
	return max;
}

// Increase heap[i] to equal key
// requires key >= heap[i]
void heap_increase_key(Heap *heap, int i, HeapKey key) {
	void *elem = vec_get(heap, i);
	if (*(HeapKey *)elem > key) {
                printf("Attempted to call heap_increase_key with a smaller key\n");
                exit(-1);
        }
	*(HeapKey *)elem = key;
	int p = heap_get_parent(i);
	while (i > 0 && heap_get_key(heap, p) < heap_get_key(heap, i)) {
		
		memswap(vec_get(heap, i), vec_get(heap, p), heap->sizeof_element);

		i = p;
		p = heap_get_parent(i);
	}
}

void heap_insert(Heap *heap, void *value, HeapKey k) {
	
	// Push value to end of heap with minimum key value
	void *elem = vec_push(heap, NULL);
	*(HeapKey *)elem = HEAPKEY_MIN;
	memcpy(elem+sizeof(HeapKey), value, heap->sizeof_element-sizeof(HeapKey));
	
	// Increase to proper key
	heap_increase_key(heap, heap->length-1, k);
}

Heap *heap_alloc(size_t sizeof_element) {
	return vec_alloc(sizeof(HeapKey)+sizeof_element);
}

void heap_free(Heap *heap) { vec_free(heap); }

void heap_build(Vec *vec) {
        for (int i = vec->length/2-1; i >= 0; i--) {
                heap_heapify(vec, i);
        }
}
