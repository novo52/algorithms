#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void print_order(int *M, int n, int i, int j, bool skip_braces) {
	if (i == j) {
		printf("A%d", i);
	} else {
		if (!skip_braces) printf("(");
		print_order(M, n, i, M[j*n+i], false);
		printf("x");
		print_order(M, n, M[j*n+i]+1, j, false);
		if (!skip_braces) printf(")");
	}
}


int mcm_optimize(int *d, int n) {
	int *M = malloc(sizeof(int)*n*n);
	//int *S = malloc(sizeof(int)*n*n);

	// M[i * n + j] = M[i, j]

	for (int dj = 0; dj < n; dj++) {
		for (int ij = 0; ij < n-dj; ij++) {
			int i = ij;    // 0
			int j = ij+dj; // 3

			//M[i, j] = metric(M, i, j);
			//printf("Searching min cost of M[%d, %d]\n", i, j);
			// Find min cost
			for (int k = i; k < j; k++) {
				//printf("cost of M[%d, %d] + M[%d, %d] + d%d*d%d*d%d\n", i, k, k+1, j, i, k+1, j+1);
				int cost = M[i*n+k] + M[(k+1)*n+j] + d[i]*d[k+1]*d[j+1];
				
				//printf("= %d + %d + %d = %d\n", M[i*n+k], M[(k+1)*n+j], d[i]*d[k+1]*d[j+1], cost);

				if (k == i || cost < M[n*i+j]) {
					M[n*i+j] = cost;
					M[n*j+i] = k; // Store S in M transposed
				}
			}
			//printf("Min found: %d\n", M[n*i+j]);
		}
	}
	
	// Retrieve final solution from table
	/*
	for (int j = 0; j < n; j++) printf("   %2d  ", j);
	printf("\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("| %4d ", M[n*i+j]);
		}
		printf("| %2d\n", i);
	}
	*/
	
	print_order(M, n, 0, n-1, true);
	printf("\n");

	int min_cost = M[n-1];
	return min_cost;
}



int main() {
	#define N 4
	int d[N+1] = {5, 2, 6, 4, 3};

	int min_cost = mcm_optimize(d, N);
	printf("Min cost: %d\n", min_cost);
	
}
