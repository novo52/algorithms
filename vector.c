#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	size_t sizeof_element; // Size of the types stored in *head
	int capacity;
	int length;
	void *head;
} Vec;

void vec_debug_print(Vec *vec);

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

// Frees the vector's internal memory and the pointer to the Vec struct
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


// Expand or contract if necessary
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

// Set capacity to length (for if you know there will be no further growth)
void vec_trim(Vec *vec) {
	vec_set_capacity(vec, vec->length);
}

void vec_set(Vec *vec, int i, void *value) {
	memcpy((char *)vec->head + i*vec->sizeof_element, value, vec->sizeof_element);
}

void *vec_get(Vec *vec, int i) {
	return (char *)vec->head + i*vec->sizeof_element;
}

void vec_push(Vec *vec, void *value) {
	vec_autoexpand(vec);
	vec_set(vec, vec->length++, value);
}

// Pops a value into the passed pointer
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

// element_print_formatted is a function that prints an element of the type stored in the vec
// in the users preferred format (ideally, without a trailing newline)
typedef void element_print_formatted(void *p);
void vec_print_formatted(Vec *vec, element_print_formatted *element_print_formatted) {
	printf("[");
	for (int i = 0; i < vec->length; i++) {
		element_print_formatted((char *)vec->head + i*vec->sizeof_element);
		if (i != vec->length-1) printf(", ");
	}
	printf("]\n");
}

// Print elements of any size as hex literals
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

void vec_debug_print(Vec *vec) {
	printf("len/cap/elem_size: %d/%d/%d",
		vec->length, vec->capacity, vec->sizeof_element);
}

typedef struct {
	int count;
	int sus[5];
	char name[10];
} user_struct;

void print_user_struct(void *p) {
	printf("{\n\tcount = %d\n\tsus = [%d, %d, %d, %d, %d]\n\tname = %.10s\n}",
		((user_struct *)p)->count,
		((user_struct *)p)->sus[0],
		((user_struct *)p)->sus[1],
		((user_struct *)p)->sus[2],
		((user_struct *)p)->sus[3],
		((user_struct *)p)->sus[4],
		((user_struct *)p)->name);
}

int main () {

	Vec *vec = vec_alloc(sizeof(user_struct));

	int count = 7;	
	for (int i = 0; i < count; i++) {
		user_struct s = {i, {i*1, i*2, i*3, i*4, i*5}, "sussy!!"};
		vec_push(vec, &s);
		vec_print_formatted(vec, &print_user_struct);

	}
}
	
