#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"

Vec *vec_alloc_sized(size_t sizeof_element, int capacity) {

	if (capacity == 0) {
		printf("Attempted to alloc Vec with 0 capacity\n");
		exit(-1);
	}

	Vec *vec = malloc(sizeof(Vec));
	if (vec == NULL) {
		printf("Failed to allocate Vec\n");
		exit(-1);
	}
	vec->capacity = 8;
	vec->length = 0;
	vec->sizeof_element = sizeof_element;
	vec->head = malloc(vec->sizeof_element * vec->capacity);
	return vec;
}
Vec *vec_alloc(size_t sizeof_element) { return vec_alloc_sized(sizeof_element, 8); }

void vec_free(Vec *vec) {
	free(vec->head);
	free(vec);
}

void vec_set_capacity(Vec *vec, int capacity) {
	
	if (capacity == 0) {
		printf("Attempted to resize Vec to 0 capacity\n");
		exit(-1);
	}
	
	vec->head = realloc(vec->head, capacity * vec->sizeof_element);
	if (vec->head == NULL) {
		printf("Failed to reallocate Vec (capacity %d to %d)\n", vec->capacity, capacity);
		vec_debug_print(vec);
		exit(-1);
	}
	vec->capacity = capacity;
}


void vec_autoexpand(Vec *vec) {
	if (vec->length == vec->capacity) {
		vec_set_capacity(vec, vec->capacity * 2);
	}
}

void vec_autocontract(Vec *vec) {
	if (vec->length*4 < vec->capacity && vec->capacity > 8) {
		vec_set_capacity(vec, vec->capacity / 2);
	}
}

void vec_trim(Vec *vec) {
	vec_set_capacity(vec, vec->length);
}

void *vec_set(Vec *vec, int i, void *value) {
	return memcpy((char *)vec->head + i*vec->sizeof_element, value, vec->sizeof_element);
}

void *vec_get(Vec *vec, int i) {
	return (char *)vec->head + i*vec->sizeof_element;
}

void *vec_push(Vec *vec, void *value) {
	vec_autoexpand(vec);
	if (value == NULL) {
		return vec_get(vec, vec->length++); // Dont copy from value
	} else {
		return vec_set(vec, vec->length++, value);
	}
}

void *vec_pop(Vec *vec) {
	if (vec->length == 0) {
		printf("Popped from empty Vec\n");
		exit(-1);
	}
	vec_autocontract(vec); // Test for contraction before popping, so the popped
			       // pointer will be in the alloc'd region of the vector
	void *ptr = vec_get(vec, --vec->length);
	return ptr;
}

void vec_print_formatted(Vec *vec, element_print_formatted *element_print_formatted) {
	printf("[");
	for (int i = 0; i < vec->length; i++) {
		element_print_formatted((char *)vec->head + i*vec->sizeof_element);
		if (i != vec->length-1) printf(", ");
	}
	printf("]\n");
}

void int_print_formatted(void *i) { printf("%d", (int *)i); }
void char_print_formatted(void *c) { printf("%c", (char *)c); }

void vec_print_hex(Vec *vec) {
	printf("[");
	for (int i = 0; i < vec->length; i++) {
		printf("0x");
		for (int j = 0; j < vec->sizeof_element; j++) {
			char byte = *((char *)vec->head + i*vec->sizeof_element + j);
			printf("%02X", byte);
		}
		if (i != vec->length-1) printf(", ");
	}
	printf("]\n");
}

void vec_print_auto(Vec *vec) {
	if (vec->sizeof_element == sizeof(int)) {
		vec_print_formatted(vec, int_print_formatted);
	} else if (vec->sizeof_element == sizeof(char)) {
		vec_print_formatted(vec, char_print_formatted);
	} else {
		vec_print_hex(vec); // Couldn't find a matching type to print
	}
}

void vec_debug_print(Vec *vec) {
	printf("len/cap/elem_size: %d/%d/%d",
		vec->length, vec->capacity, vec->sizeof_element);
}
	
