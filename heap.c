#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>


typedef struct Heap {
	int *root;
	int size;
} Heap;

void heap_print_array(Heap *heap) {
	printf("[");
	for (int i = 0; i < heap->size; i++) {
		printf("%d", heap->root[i]);
		if (i != heap->size-1) printf(", ");
	}
	printf("]\n");
}

int heap_get_left(int j) 	{ return (j+1)*2-1; }
int heap_get_right(int j) 	{ return (j+1)*2; }
int heap_get_parent(int j) 	{ return (j+1)/2-1; }

// Requires the subtree rooted at i to be an almost-heap
// Turns it into a heap
void heap_heapify(Heap* heap, int i) {
	int left = heap_get_left(i);
	int right = heap_get_right(i);
	int largest = i;
	


	// largest = max of existing children
	if (left < heap->size && heap->root[left] > heap->root[largest]) largest = left;
	if (right < heap->size && heap->root[right] > heap->root[largest]) largest = right;

	//printf("i = %d (v %d), left = %d (v %d), right = %d (v %d), largest = %d (v %d)\n", i, heap->root[i], left, heap->root[left], right, heap->root[right], largest, heap->root[largest]);

	// if we have a larger child, swap and fix the subtree
	if (largest != i) {
		int temp = heap->root[i];
		heap->root[i] = heap->root[largest];
		heap->root[largest] = temp;
		heap_heapify(heap, largest);
	}
}

int heap_extract_max(Heap *heap) {
	int max = heap->root[0];
	heap->root[0] = heap->root[heap->size-1]; // Put the last one at the top
	heap->size--; // Delete the last one
	if (heap->size > 0) heap_heapify(heap, 0);
	return max;
}

// Increase heap[i] to equal key
// requires key >= heap[i]
void heap_increase_key(Heap *heap, int i, int key) {
	heap->root[i] = key;
	int p = heap_get_parent(i);
	while (i > 0 && heap->root[p] < heap->root[i]) {
		int temp = heap->root[i];
		heap->root[i] = heap->root[p];
		heap->root[p] = temp;

		i = p;
		p = heap_get_parent(i);
	}
}

void heap_insert(Heap *heap, int i) {
	heap->size++;
	heap->root[heap->size-1] = INT_MIN;
	heap_increase_key(heap, heap->size-1, i);
}

// Build a heap out of array, using array as the internal repreention of the heap
// Ownership of array is taken
Heap* heap_build_from_array(int *array, int size) {
	Heap* heap = malloc(sizeof(Heap));
	heap->root = array;
	heap->size = size;

	for (int i = size/2-1; i >= 0; i--) {
		heap_heapify(heap, i);
	}
	return heap;
}



int main() {
	int array[10] = {16, 14, 10, 8, 7, 9, 3, 2, 4};
	Heap* heap = heap_build_from_array(array, 10);
	heap_print_array(heap);
	heap->size--;
	heap_insert(heap, 15);
	printf("Inserted 15\n");
	heap_print_array(heap);
}
