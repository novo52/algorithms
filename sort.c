#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "util.h"
#include "sort.h"

void quicksort_int(int *arr, int n, bool desc) {
	if (n <= 1) return;
	
	// Partition
	int i = -1;
	for (int j = 0; j < n; j++) {
		if ((!desc && (arr[j] <= arr[n-1])) ||
		     (desc && (arr[j] >= arr[n-1]))) {
			i++;
			int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}	

	quicksort_int(arr, i, desc);
	quicksort_int(arr+i+1, n-i-1, desc);
}


void quicksort(void *arr, int n, size_t sizeof_element, ComparisonFn comp, bool desc) {
	if (n <= 1) return;

	// Partition
	int i = -1;
	for (int j = 0; j < n; j++) {
		int c = comp(arr + j*sizeof_element, arr + (n-1)*sizeof_element);
		if ((!desc && (c <= 0)) ||
		     (desc && (c >= 0))) {
			i++;
			if (i != j) memswap(arr + i*sizeof_element, arr + j*sizeof_element, sizeof_element);
		}
	}	

	quicksort(arr, i, sizeof_element, comp, desc);
	quicksort(arr+(i+1)*sizeof_element, n-i-1, sizeof_element, comp, desc);
}

