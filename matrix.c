#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "matrix_chain_multiplication.h"
#include "matrix.h"
#include "util.h"

Matrix *matrix_alloc(int size_i, int size_j) {
	Matrix *matrix = malloc(sizeof(Matrix));
	matrix->size_i = size_i;
	matrix->size_j = size_j;
	matrix->data = malloc(sizeof(int)*size_i*size_j);
	return matrix;
}

void matrix_free(Matrix *matrix) {
	free(matrix->data);
	free(matrix);
}

int *matrix_getp(Matrix *matrix, int i, int j) { return &matrix->data[i*matrix->size_j+j]; }

void matrix_print(Matrix *matrix) {

	for (int i = 0; i < matrix->size_i; i++) {
		for (int j = 0; j < matrix->size_j; j++) {
			printf("| %4d ", *matrix_getp(matrix, i, j));
		}
		printf("|\n");
	}
}

bool matrix_equal(Matrix *m1, Matrix *m2) {
	if (m1->size_i != m2->size_i) return false;
	if (m1->size_j != m2->size_j) return false;
	for (int i = 0; i < m1->size_i; i++) {
		for (int j = 0; j < m1->size_j; j++) {
			if (m1->data[i*m1->size_j+j] != m2->data[i*m1->size_j+j]) {
				return false;
			}
		}
	}
	return true;
}

Matrix *matrix_multiply(Matrix *m1, Matrix *m2) {
	
	Matrix *result = matrix_alloc(m1->size_i, m2->size_j);
	
	for (int i = 0; i < result->size_i; i++) {
		for (int j = 0; j < result->size_j; j++) {
			*matrix_getp(result, i, j) = 0;
			for(int k = 0; k < m1->size_j; k++) {
				*matrix_getp(result, i, j) += 
					*matrix_getp(m1, i, k) *
					*matrix_getp(m2, k, j);
			}
		}
	}
	return result;
}

int determine_max_matrix_size_recursive(int *order, int *d, int a, int b, int *k, int l, bool left) {
	if (a >= b) return 0;
	printf("called with a=%d b=%d k=%d l=%d left?=%d\n", a, b, *k, l, left);

	int product_size = d[a]*d[b+1]; // size of (Aa*Aa+1*...*Ab)

	int s = order[(*k)++];
	int left_max_size = determine_max_matrix_size_recursive(order, d, a, s, k, l+1, true); 
	int right_max_size = determine_max_matrix_size_recursive(order, d, s+1, b, k, l+1, false);
	printf("A%dx...xA%d is size %dx%d = %d\n", a, b, d[a], d[b+1], product_size);
	int max_size = product_size;
	if (max_size < left_max_size) max_size = left_max_size;
	if (max_size < right_max_size) max_size = right_max_size;
	return max_size;
}

int determine_max_matrix_size(int *order, int *d, int n) {
	int k = 0;
	return determine_max_matrix_size_recursive(order, d, 0, n-1, &k, 0, true);
}

Matrix *matrix_chain_multiply_recursive(int *order, Matrix **matrices, int a, int b, int *k) {
	if (a == b) return matrices[a];
	if (a > b) {
		printf("Uhhhhhh.......\n");
		return matrices[b];
	}

        //printf("called with a=%d b=%d k=%d\n", a, b, *k);

        int s = order[(*k)++];
        Matrix *m1 = matrix_chain_multiply_recursive(order, matrices, a, s, k); 
        Matrix *m2 = matrix_chain_multiply_recursive(order, matrices, s+1, b, k);
        //printf(" is size %dx%d = %d\n", a, b, d[a], d[b+1], product_size);
	
	Matrix *product = matrix_multiply(m1, m2);

	if (a != s) matrix_free(m1);
	if (s+1 != b) matrix_free(m2);

	return product;
}

Matrix *matrix_chain_multiply_array(int n, Matrix **matrices) {
	// Extract ds
	int *d = malloc(sizeof(int)*(n+1));
	d[0] = matrices[0]->size_i;
	for (int i = 0; i < n; i++) d[i+1] = matrices[i]->size_j;

	// Determine optimal multiplication order
	int *order = mcm_optimize(d, n);
	//print_int_array(order, n-1);
	//printf("Optimal order: ");
	//print_order_extracted(order, 0, n-1, 0, true);
	//printf("\n");

	int k = 0;
	Matrix *result = matrix_chain_multiply_recursive(order, matrices, 0, n-1, &k); 
	
	free(d);
	free(order);

	return result;
}

Matrix *matrix_chain_multiply(int n, ...) {

	va_list ptr;
	va_start(ptr, n);

	Matrix **matrices = malloc(sizeof(Matrix *)*n);
	
	for (int i = 0; i < n; i++) {
		matrices[i] = va_arg(ptr, Matrix *);
		if (i != 0 && matrices[i]->size_i != matrices[i-1]->size_j) {
			printf("Matrix size mismatch between m%d, m%dd\n", i-1, i);
			exit(-1); // BOOM
		}
	}
	
	Matrix *result = matrix_chain_multiply_array(n, matrices);
	
	free(matrices);

	return result;
}
