#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include <stdbool.h>

typedef struct {
	int size_i;
	int size_j;
	int *data;
} Matrix;

// Alloc and free (operate on Matrix *)
Matrix *matrix_alloc(int size_i, int size_j);
void matrix_free(Matrix *matrix);

// Get a pointer to matrix[i, j] for assignment or to get the value
int *matrix_getp(Matrix *matrix, int i, int j);

void matrix_print(Matrix *matrix);

// Test for matrix equality
bool matrix_equal(Matrix *m1, Matrix *m2);

// Classic matrix multiplication
Matrix *matrix_multiply(Matrix *m1, Matrix *m2);

// Multiply every matrix in **matrices from left to right
Matrix *matrix_chain_multiply_array(int n, Matrix **matrices);

// Like above, but with arguments instead of an array
//     *matrix_chain_multiply(int n, Matrix *m1, Matrix *m2, ..., Matrix *mn);
Matrix *matrix_chain_multiply(int n, ...);

#endif
