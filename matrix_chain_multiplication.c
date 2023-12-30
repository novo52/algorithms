#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "vector.h"
#include "util.h"


void extract_order(int *M, Vec *v, int n, int i, int j) {
	if (i == j) {
		//printf("A%d", i);
	} else {
		int k = M[j*n+i];
		vec_push(v, &k);
		extract_order(M, v, n, i, k);
		extract_order(M, v, n, k+1, j);
	}
}

void print_order_extracted(int *x, int i, int j, int k, bool skip_braces) {
	if (i == j) {
		printf("A%d", i);
		return;
	}

	if (!skip_braces) printf("(");
	print_order_extracted(x, i, x[k], k+1, false);
	printf("x");
	print_order_extracted(x, x[k]+1, j, k+1+x[k]-i, false);
	if (!skip_braces) printf(")");	
}

int *mcm_optimize(int *d, int n) {
	int *M = malloc(sizeof(int)*n*n);

	// M[i * n + j] = M[i, j]

	for (int dj = 0; dj < n; dj++) {
		for (int ij = 0; ij < n-dj; ij++) {
			int i = ij;    // 0
			int j = ij+dj; // 3

			//M[i, j] = metric(M, i, j);
			
			// Find min cost
			M[n*i+j] = INT_MAX;
			for (int k = i; k < j; k++) {
				int cost = M[i*n+k] + M[(k+1)*n+j] + d[i]*d[k+1]*d[j+1];
				if (cost < M[n*i+j]) {
					M[n*i+j] = cost;
					M[n*j+i] = k; // Store S in M transposed
				}
			}
		}
	}
	
	Vec *result = vec_alloc_sized(sizeof(int), n-1);
	extract_order(M, result, n, 0, n-1);
	int *data = result->head;
	free(result); // Free vec but not the data
	free(M);
	return data;
}

