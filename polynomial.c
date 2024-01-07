#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tgmath.h>

#include "polynomial.h"

double eval_quadratic(double a, double b, double c, double x) {
	return a*x*x + b*x + c;
}

void solve_quadratic(double a, double b, double c, double solutions[2], int *solution_count) {

	double det = b*b - 4.0*a*c;

	*solution_count = 0; // There exists at least one solution
	if (det >= 0) solutions[(*solution_count)++] = ( -b - sqrt(det) )/( 2.0*a );
	if (det >  0) solutions[(*solution_count)++] = ( -b + sqrt(det) )/( 2.0*a );

}

void solve_quadratic_vertex(double a, double b, double c, 
		double *solution_x, double *solution_y) {
	// The vertex is where derivative = 0
	
	*solution_x = -b/(2.0*a);
	*solution_y = eval_quadratic(a, b, c, *solution_x);
}

double eval_cubic(double a, double b, double c, double d, double x) {
	return a*x*x*x + b*x*x + c*x + d;
}

void solve_cubic(double a_, double a2, double a1, double a0, 
		double solutions[3], int *solution_count) {
	// Divide by the leading coeffient to make it an implicit one
	a0 /= a_;
	a1 /= a_;
	a2 /= a_;

	double q = a1/3.0-a2*a2/9.0;
	double r = (a1*a2-3.0*a0)/6.0-a2*a2*a2/27.0;

	double det = r*r+q*q*q;
	if (det > 0) {
		*solution_count = 1;
		double A = cbrt(fabs(r)+sqrt(det));
		double t1 = A - q/A;
		if (r < 0) t1 = -t1;
		
		solutions[0] = t1-a2/3.0;
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

		solutions[0] = 2.0 * sqrt_q * cos(p3)-a2/3.0;
		solutions[1] = 2.0 * sqrt_q * cos(p2)-a2/3.0;
		solutions[2] = 2.0 * sqrt_q * cos(p1)-a2/3.0;

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
