#ifndef POLYNOMIAL_HEADER
#define POLYNOMIAL_HEADER

typedef struct Polynomial {
	int d;
	double *p; // p[d+1]
} Polynomial;

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
void solve_cubic(double a_, double a2, double a1, double a0, 
		double solutions[3], int *solution_count);

// for f(x) = a*x^3 + b*x^2 + c*x + d
// solution of f'(x) = 0
void solve_cubic_vertices(double a, double b, double c, double d, 
		double solutions[2], int *solution_count);

void solve_polynomial(Polynomial *polynomial, double *solutions, int *solution_count);

#endif
