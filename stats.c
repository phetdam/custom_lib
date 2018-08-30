/*
 * stats.c
 *
 * contains (soon!) several useful statistics functions that C lacks.
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stats.h>

// normal cdf function; uses abramowitz' and stegun's approximation 7.1.28 with absolute error 
// 3 * 10^(-7); x is the value of which we wish to find P(X < x), mu is mean (X ~ N(mu, s^2)),
// and s is standard deviation. link to their text: http://people.math.sfu.ca/~cbm/aands/toc.htm
double normalcdf(double x, double mu, double s) {
    assert(s >= 0);
    // normalize x
    double n_x = (x - mu) / s / sqrt(2);
    return 0.5 + 0.5 * (1.0 - pow(1 + A_1 * n_x + A_2 * pow(n_x, 2) + A_3 * pow(n_x, 3) + \
				  A_4 * pow(n_x, 4) + A_5 * pow(n_x, 5) + A_6 * pow(n_x, 6), -16));
}
