#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tgmath.h>

#include "polynomial.h"

double eval_quadratic(double a, double b, double c, double x) {
	return a*x*x + b*x + c;
}

void solve_quadratic(double a, double b, double c, double solutions[2], int *solution_count) {
	if (a == 0) {
		*solution_count = 1;
		solutions[0] = -c/b;
		solutions[1] = 0.0;
		return;
	}

	double det = b*b - 4.0*a*c;

	*solution_count = 0;
	solutions[0] = 0.0;
	solutions[1] = 0.0;
	if (det >= 0) solutions[(*solution_count)++] = ( -b - sqrt(det) )/( 2.0*a );
	if (det >  0) solutions[(*solution_count)++] = ( -b + sqrt(det) )/( 2.0*a );

}

double solve_quadratic_vertex(double a, double b, double c) {
	if (a == 0) return -c/b;
	else return -b/(2.0*a); // Solution of the derivative
}

double eval_cubic(double a, double b, double c, double d, double x) {
	return a*x*x*x + b*x*x + c*x + d;
}

// https://quarticequations.com/Cubic.pdf
void solve_cubic(double a, double b, double c, double d, 
		double solutions[3], int *solution_count) {
	if (a == 0) {
		solve_quadratic(b, c, d, solutions, solution_count);
		return;
	}

	// Divide by the leading coeffient to make it an implicit 1
	b /= a;
	c /= a;
	d /= a;

	double q = c/3.0-b*b/9.0;
	double r = (c*b-3.0*d)/6.0-b*b*b/27.0;

	double det = r*r+q*q*q;
	if (det > 0) {
		*solution_count = 1;
		double A = cbrt(fabs(r)+sqrt(det));
		double t1 = A - q/A;
		if (r < 0) t1 = -t1;
		
		solutions[0] = t1-b/3.0;
		solutions[1] = 0.0;
		solutions[2] = 0.0;
	} else {
		*solution_count = 3;
		
		double t = 0;
		double sqrt_q = 0;
		if (q < 0) {
			sqrt_q = sqrt(-q);
			t = acos(r/(sqrt_q*sqrt_q*sqrt_q));
		}
		double p1 = t/3.0;
		double p2 = p1-2.0*M_PI/3.0;
		double p3 = p1+2.0*M_PI/3.0;
			
		printf("p1=%lf, p2=%lf, p3=%lf\n", p1, p2, p3);

		solutions[0] = 2.0 * sqrt_q * cos(p3)-b/3.0;
		solutions[1] = 2.0 * sqrt_q * cos(p2)-b/3.0;
		solutions[2] = 2.0 * sqrt_q * cos(p1)-b/3.0;

		if (solutions[0] == solutions[1]) {
			solutions[1] = solutions[2];
			solutions[2] = 0.0;
			*solution_count = 2;
		} else if (solutions[1] == solutions[2]) {
			*solution_count = 2;
			solutions[2] = 0.0;
		}
	}
}

void solve_cubic_vertices(double a, double b, double c, double d, 
		double solutions[2], int *solution_count) {
	// Solve the derivative
	solve_quadratic(3.0*a, 2.0*b, c, solutions, solution_count);
}

void solve_polynomial(Polynomial *polynomial, double *solutions, int *solution_count) {
	if (polynomial->d == 0) {
		*solution_count = 1;
		solutions[0] = polynomial->p[0];
	} else if (polynomial->d == 1) {
		solve_quadratic(0.0, polynomial->p[1], polynomial->p[0],
				solutions, solution_count);
	} else if (polynomial->d == 2) {
		solve_quadratic(polynomial->p[2], polynomial->p[1], polynomial->p[0],
				solutions, solution_count);
	} else if (polynomial->d == 3) {
		solve_cubic(polynomial->p[3], polynomial->p[2], polynomial->p[1], 
				polynomial->p[0], solutions, solution_count);
	}
}
