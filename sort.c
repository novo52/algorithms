#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void quicksort(int *arr, int n, bool desc) {
	if (n <= 1) return;
	
	// Partition
	int i = -1;
	for (int j = 0; j < n; j++) {
		if (!desc && arr[j] <= arr[n-1] ||
		     desc && arr[j] >= arr[n-1]) {
			i++;
			int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}	

	quicksort(arr, i, desc);
	quicksort(arr+i+1, n-i-1, desc);
}
