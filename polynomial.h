#ifndef POLYNOMIAL_HEADER
#define POLYNOMIAL_HEADER
#include <complex.h>

typedef struct Polynomial {
	int d;
	double *p; // p[d+1]
} Polynomial;

// Allocate and free a polynomial
Polynomial *polynomial_alloc(int degree);
void polynomial_free(Polynomial *poly);

// Takes ownership of *p
// Note: p[0] = dth term
Polynomial *polynomial_alloc_from_terms(int degree, double *p);

// Print in human readable format
void polynomial_print(Polynomial *polynomial);

// f(x) = a*x^2 + b*x + c
double eval_quadratic(double a, double b, double c, double x);

// solutions of a*x^2 + b*x + c = 0 for x
// places 0, 1, or 2 unique solutions in the first solution_count indices of solutions[]
void solve_quadratic(double a, double b, double c, double solutions[2], int *solution_count);

// for f(x) = a*x^2 + b*x + c
// solution of f'(x) = 0
double solve_quadratic_vertex(double a, double b, double c);

// f(x) = a*x^3 + b*x^2 + c*x + d
double eval_cubic(double a, double b, double c, double d, double x);

// solutions of a*x^3 + b*x^2 + c*x + d = 0 for x
// places 1, 2, or 3 unique solutions in the first solution_count indices of solutions[]
void solve_cubic(double a, double b, double c, double d, 
		double solutions[3], int *solution_count);

// for f(x) = a*x^3 + b*x^2 + c*x + d
// solution of f'(x) = 0
void solve_cubic_vertices(double a, double b, double c, double d, 
		double solutions[2], int *solution_count);

void solve_polynomial(Polynomial *polynomial, double complex *solutions);

Polynomial *polynomial_derivative(Polynomial *polynomial);
Polynomial *polynomial_mul(Polynomial *p1, Polynomial *p2);
void polynomial_div(Polynomial *num, Polynomial *denom,
		Polynomial **quot, Polynomial **remainder);

// Evaluate the polynomial at z
double complex polynomial_eval(Polynomial *polynomial, complex double z);
#endif
