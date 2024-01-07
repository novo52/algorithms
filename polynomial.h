#ifndef POLYNOMIAL_HEADER
#define POLYNOMIAL_HEADER

// f(x) = a*x^2 + b*x + c
double eval_quadratic(double a, double b, double c, double x);

// solutions of a*x^2 + b*x + c = 0 for x
// places 0, 1, or 2 unique solutions in the first solution_count indices of solutions[]
void solve_quadratic(double a, double b, double c, double solutions[2], int *solution_count);

// for f(x) = a*x^2 + b*x + c
// solution of f'(x) = 0
void solve_quadratic_vertex(double a, double b, double c, 
		double *solution_x, double *solution_y);

// f(x) = a*x^3 + b*x^2 + c*x + d
double eval_cubic(double a, double b, double c, double d, double x);

// solutions of a*x^3 + b*x^2 + c*x + d = 0 for x
// places 1, 2, or 3 unique solutions in the first solution_count indices of solutions[]
void solve_cubic(double a_, double a2, double a1, double a0, 
		double solutions[3], int *solution_count);

#endif
