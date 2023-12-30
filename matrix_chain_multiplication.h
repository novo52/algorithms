
#ifndef MATRIX_CHAIN_MULTIPLICATION_HEADER
#define MATRIX_CHAIN_MULTIPLICATION_HEADER


// For matrices A1, A2 ... An with dimensions d0xd1, d1xd2, d3xd4 ... dn-1xdn
// This function computes the optimal order to reduce the time complexity of the
// operation A1xA2x...xAn (matrix chain multiplication)
//
// Returns: an array of ints n-1 long, containing the indices to subdivide the multiplication
// problem. For example, a return value of [0, 2, 1] would corrispond to the optimal
// order A1x((A2xA3)xA4) (split at x0 to A1x(A2xA3xA4) etc). These idices are ordered by
// depth first traversal, so they can be passed into a df binary multiplication easily.
int *mcm_optimize(int *d, int n);

// Print the output of mcm_optimize in a human-readbale format for debugging purposes
void print_order_extracted(int *x, int i, int j, int k, bool skip_braces);

#endif
