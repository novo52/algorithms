#include "util.h"
#include <stdio.h>

void memswap(void *p1, void *p2, size_t size) {
        void *tp = malloc(size);
        memcpy(tp, p1, size);
        memcpy(p1, p2, size);
        memcpy(p2, tp, size);
        free(tp);
}

void *memdup(void *p, size_t size) {
	void *d = malloc(size);
	memcpy(d, p, size);
	return d;
}

void print_int_array(int *arr, int n) {
        printf("[");
        for (int i = 0; i < n; i++) {
                printf("%d", arr[i]);
                if (i != n-1) printf(", ");
        }
        printf("]");
}

void print_double_array(double *arr, double n) {
        printf("[");
        for (int i = 0; i < n; i++) {
                printf("%lf", arr[i]);
                if (i != n-1) printf(", ");
        }
        printf("]");
}
