#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Vec {
	size_t sizeof_element; // Size of the types stored in *head
	int capacity;
	int length;
	void *head;
} Vec;

// Allocate a vector
Vec *vec_alloc_sized(size_t sizeof_element, int capacity);
Vec *vec_alloc(size_t sizeof_element);

// Frees the vector's internal memory and the pointer to the Vec struct
void vec_free(Vec *vec);

void vec_set_capacity(Vec *vec, int capacity);

// Expand the capacity if the length has reached capacity
// Automatically called by vec_push
void vec_autoexpand(Vec *vec);

// Contract the capacity if less than 25% is used
// Not called automatically, if contraction is desired, call this **before** vec_pop
void vec_autocontract(Vec *vec);

// Set capacity to length (for if you know there will be no further growth)
void vec_trim(Vec *vec);

// Setter and getter for elements
// vec_get returns a pointer to the value, to get a value use value = *vec_get(vec, i);
void *vec_set(Vec *vec, int i, void *value);
void *vec_get(Vec *vec, int i);

// Push and pop both return pointers to the element
// If a null pointer is passed to push, it will not copy any memory, but still increase the
// length (basically, push an empty value that doesn't overwrite)
void *vec_push(Vec *vec, void *value);
void *vec_pop(Vec *vec);

// element_print_formatted is any user supplied function that prints one element of the type
// stored in the vector, in the users preferred format (ideally, without a trailing newline)
typedef void element_print_formatted(void *p);
void vec_print_formatted(Vec *vec, element_print_formatted *element_print_formatted);
void int_print_formatted(void *i);
void char_print_formatted(void *c);

// Print elements of any size or type as hex literals
void vec_print_hex(Vec *vec);

// Guess the type and print formatted using that, otherwise print hex
void vec_print_auto(Vec *vec);

// Print information about the vector, but not its contents
void vec_debug_print(Vec *vec);

#endif
