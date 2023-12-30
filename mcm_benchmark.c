
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "matrix.h"
#include "matrix_chain_multiplication.h"

void test(int C, int min_size, int max_size, bool *equal, double *time1, double *time2) {
	
	// Generate C random input matrices
	int d[C+1];
	for (int i = 0; i < C+1; i++) {
		if (min_size != max_size) {
			d[i] = rand() % (max_size-min_size) + min_size + 1;
		} else {
			d[i] = min_size;
		}
	}
	
	Matrix *matrices[C];
	for (int k = 0; k < C; k++) {
		matrices[k] = matrix_alloc(d[k], d[k+1]);
		for (int i = 0; i < matrices[k]->size_i; i++) {
			for (int j = 0; j < matrices[k]->size_j; j++) {
				*matrix_getp(matrices[k], i, j) = rand();
			}
		}
	}

	// Naive multiply
	clock_t begin1 = clock();
	Matrix *prev_result = matrices[0];
	Matrix *result1;
	for (int i = 1; i < C; i++) {
		result1 = matrix_multiply(prev_result, matrices[i]);
		if (i != 1) matrix_free(prev_result);
		prev_result = result1;
	}
	clock_t end1 = clock();

	// Optimal multiply
	clock_t begin2 = clock();
	Matrix *result2 = matrix_chain_multiply_array(C, matrices);
	clock_t end2 = clock();
	
	// Return values
	*time1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;
	*time2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
	*equal = matrix_equal(result1, result2);

	// Free
	for (int i = 0; i < C; i++) matrix_free(matrices[i]);
	matrix_free(result1);
	matrix_free(result2);
}

void test_min() {
	
	const int T = 20;
	const int I = 70;

	double (*min_trial_time)[T] = malloc(sizeof(double[2][T]));
	for (int t = 0; t < T; t++) 
		for (int x = 0; x < 2; x++) 
			min_trial_time[x][t] = INFINITY;

	int k = 0;
	for (int i = 0; i < I; i++) {	
		for (int t = 0; t < T; t++) {
			srand(i);
			double times[2];
			bool equal;
			printf("%2d [%4d/%4d] ", t, ++k, T*I);
			test(30, 10, (t+2)*25, &equal, &times[0], &times[1]);

			printf("Equal: %d, %.4lfs vs %.4lfs\n", equal, times[0], times[1]);
			for (int x = 0; x < 2; x++) {
				if (times[x] < min_trial_time[x][t]) {
					min_trial_time[x][t] = times[x];
				}
			}
		}
	}
	
	for (int i = 0; i < T; i++) {
		printf("%2d | %lf vs %lf (x%.2lf)\n", i, min_trial_time[0][i], min_trial_time[1][i], min_trial_time[0][i] / min_trial_time[1][i]);
	}
}

void test_avg() {
	
	const int T = 10;
	const int I = 100;

	double (*avg_trial_time)[T] = malloc(sizeof(double[2][T]));
	for (int t = 0; t < T; t++) 
		for (int x = 0; x < 2; x++) 
			avg_trial_time[x][t] = 0;

	int k = 0;
	for (int i = 0; i < I; i++) {	
		for (int t = 0; t < T; t++) {
			srand(i);
			double times[2];
			bool equal;
			printf("%2d [%4d/%4d] ", t, ++k, T*I);
			test(30, 10, (t+2)*40, &equal, &times[0], &times[1]);

			printf("Equal: %d, %.4lfs vs %.4lfs\n", equal, times[0], times[1]);
			for (int x = 0; x < 2; x++) {
				avg_trial_time[x][t] += times[x];
			}
		}
	}

	
	
	for (int t = 0; t < T; t++) {
		for (int x = 0; x < 2; x++) avg_trial_time[x][t] /= I;
		printf("%2d | %lf vs %lf (x%.2lf)\n", t, avg_trial_time[0][t], avg_trial_time[1][t], avg_trial_time[0][t] / avg_trial_time[1][t]);
	}
}

int main() {
	test_avg();
}

