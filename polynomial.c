#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tgmath.h>
#include <complex.h>
#include <stdbool.h>
#include <limits.h>

#include "polynomial.h"

Polynomial *polynomial_alloc(int degree) {
	double *p = calloc(degree+1, sizeof(double));
	Polynomial *poly = malloc(sizeof(Polynomial));
	poly->d = degree;
	poly->p = p;
	return poly;
}

Polynomial *polynomial_alloc_from_terms(int degree, double *p) {
	Polynomial *poly = malloc(sizeof(Polynomial));
	poly->d = degree;
	poly->p = p;
	return poly;
}

void polynomial_free(Polynomial *poly) {
	free(poly->p);
	free(poly);
}

void polynomial_print(Polynomial *polynomial) {
	for (int d = polynomial->d; d >= 0; d--) {
		if (polynomial->p[d] >= 0.0) {
			if (d != polynomial->d) printf(" + ");
			printf("%.3lf", polynomial->p[d]);

		} else {
			if (d != polynomial->d) printf(" - ");
			printf("%.3lf", -polynomial->p[d]);
		}
		if (d >= 2) printf("*x^%d", d);
		if (d == 1) printf("*x");
	}
}

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

// https://en.wikipedia.org/wiki/Geometrical_properties_of_polynomial_roots#Lagrange's_and_Cauchy's_bounds
double cauchy_roots_upper_bound(Polynomial *poly) {
	double max_value = 0.0;
	for (int i = 0; i < poly->d; i++) {
		double value = abs(poly->p[i]);
		if (value > max_value) {
			max_value = value;
		}
	}

	return 1.0 + max_value/poly->p[poly->d];
}

void roots_bound(Polynomial *polynomial, double *upper, double *lower) {
	// Upper is trivial
	*upper = cauchy_roots_upper_bound(polynomial);
	
	// Lower is symmetric
	// flip the terms of the polynomial, find the upper root, and take the inverse
	double *rp = malloc(sizeof(double) * (polynomial->d+1));
	for (int i = 0; i <= polynomial->d; i++) rp[i] = polynomial->p[polynomial->d-i];
	double *p = polynomial->p;
	polynomial->p = rp;
	*lower = 1.0/cauchy_roots_upper_bound(polynomial);
	polynomial->p = p;
	free(rp);
}

double polynomial_get_term(Polynomial *polynomial, int i) {
	if (i <= polynomial->d) return polynomial->p[i];
	return 0.0;
}

// Trim zeros off the polynomial
void polynomial_shrink(Polynomial *polynomial) {
	for (; polynomial->d >= 0; polynomial->d--) if (polynomial->p[polynomial->d] != 0.0) break;
}

Polynomial *polynomial_derivative(Polynomial *polynomial) {
	Polynomial *answer = polynomial_alloc(polynomial->d-1);
	// Apply power rule to every term
	for (int i = 0; i < polynomial->d; i++) answer->p[i] = (double)(i+1)*polynomial->p[i+1];
	return answer;
}

Polynomial *polynomial_sum(Polynomial *p1, Polynomial *p2) {
	int d = p1->d;
	if (p2->d > d) d = p2->d;
	Polynomial *sum = polynomial_alloc(d);
	for (int i = 0; i <= d; i++) 
		sum->p[i] = polynomial_get_term(p1, i) + polynomial_get_term(p2, i);
	polynomial_shrink(sum);
	return sum;
}

Polynomial *polynomial_sub(Polynomial *p1, Polynomial *p2) {
	int d = p1->d;
	if (p2->d > d) d = p2->d;
	Polynomial *dif = polynomial_alloc(d);
	for (int i = 0; i <= d; i++)
		dif->p[i] = polynomial_get_term(p1, i) - polynomial_get_term(p2, i);
	polynomial_shrink(dif);
	return dif;
}

Polynomial *polynomial_mul(Polynomial *p1, Polynomial *p2) {
	int d = (p1->d+1)*(p2->d+1)-1;
	Polynomial *prod = polynomial_alloc(d);
	for (int a = 0; a <= p1->d; a++) {
		for (int b = 0; b <= p2->d; b++) {
			prod->p[a+b] += p1->p[a]*p2->p[b];
		}
	}
	polynomial_shrink(prod);
	return prod;
}

bool polynomial_is_zero(Polynomial *polynomial) {
	return polynomial->d == 0 && polynomial->p[0] == 0.0;
}

void polynomial_div(Polynomial *num, Polynomial *denom,
		Polynomial **quot, Polynomial **remainder) {

	// q = 0
	*quot = polynomial_alloc(0);
	(*quot)->p[0] = 0.0;

	// r = n
	*remainder = polynomial_alloc(num->d);
	for (int i = 0; i <= num->d; i++) (*remainder)->p[i] = num->p[i];

	while(!polynomial_is_zero(*remainder) && (*remainder)->d >= denom->d) {
		// t = lead(r) / lead(d)
		int d = (*remainder)->d - denom->d;
		double val = (*remainder)->p[(*remainder)->d] / denom->p[denom->d];
		Polynomial *t = polynomial_alloc(d);
		t->p[d] = val;
	
		// Calculate new values
		Polynomial *new_quot = polynomial_sum(*quot, t);
		Polynomial *intermediate = polynomial_mul(t, denom);
		Polynomial *new_remainder = polynomial_sub(*remainder, intermediate);

		// Replace old values
		polynomial_free(*quot);
		*quot = new_quot;

		polynomial_free(*remainder);
		*remainder = new_remainder;

		// Free
		polynomial_free(t);
		polynomial_free(intermediate);
	}
}

double complex polynomial_eval(Polynomial *polynomial, complex double z) {
	double complex result = 0.0 + 0.0 * I;
	for (int i = 0; i <= polynomial->d; i++) {
		double complex term = polynomial->p[i];
		for (int j = 0; j < i; j++) {
			term *= z;
		}
		result += term;
	}
	return result;
}
		



// todo: use a more numerically stable method
// file:///home/tyler/Downloads/bini1996.pdf section 3
//void poly_over_derivative_at_x(Polynomial *polynomial, double x) {

double complex alberth_update(Polynomial *p, Polynomial *d, 
		double complex *solutions, int k) {
	double complex z = solutions[k];
	
	double complex repulsion = 0.0;
	for (int j = 0; j < p->d-1; j++) {
		if (k == j) continue;
		repulsion += 1.0 / (z - solutions[j]);
	}

	double complex p_dp_z = 
			polynomial_eval(p, z) / polynomial_eval(d, z);

	double complex w = p_dp_z / (1 - p_dp_z*repulsion);
	
	return w;
}


// https://en.wikipedia.org/wiki/Aberth_method
void solve_polynomial(Polynomial *polynomial, double complex *solutions) {

	Polynomial *derivative = polynomial_derivative(polynomial);
	
	double upper_bound, lower_bound;
	roots_bound(polynomial, &upper_bound, &lower_bound);

	// Generate d-1 approximation roots equally spaced around the origin
	// with random magnitudes between the upper and lower bounds
	rand();
	for (int i = 0; i < polynomial->d-1; i++) {
		double t = (double)i/(polynomial->d-1)*2.0*M_PI;
		double random = (double)rand()/(double)INT_MAX;
		double r = random*(upper_bound-lower_bound)+lower_bound;
		double a = r*cos(t);
		double b = r*sin(t);
		solutions[i] = a + b*I;
	}


	double complex *offsets = malloc(sizeof(double complex) * polynomial->d);

	for (int i = 0; i < 20; i++) {
		for (int k = 0; k < polynomial->d-1; k++) {
			offsets[k] = alberth_update(polynomial, derivative, solutions, k);
		}
		for (int k = 0; k < polynomial->d-1; k++) {
			solutions[k] -= offsets[k];
		}
	}
	
	free(offsets);

}



/*
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
*/
