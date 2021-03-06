/*
 * stats.c
 * contains (soon!) several useful statistics functions that C lacks.
 *
 * Changelog:
 *
 * 09-10-2018
 * corrected asymmetric behavior of normalcdf function; replaced original erf implementation 
 * with a different and more accurate numerical approximation.
 *
 * 08-30-2018
 * added normalpdf function, makefile, main function; edited normalcdf function and stats files
 *
 * 08-29-2018
 * initial completion of normalcdf function, using abramowitz and stegun's approximation for erf
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "stats.h"

// normal cdf function; uses abramowitz' and stegun's approximation 26.2.19 for normal cdf with
// absolute error 1.5 * 10^(-7) http://people.math.sfu.ca/~cbm/aands/page_932.htm
// note that the approximation was defined to work only where x >= 0, so for values of x < mu,
// we approximate P(X < x | x < mu) == 1 - P(X < abs(x))
// since the range of x is from -inf to x, subtract by 0.5 to get P(0 < X < x)
// use mu = STD_MU and s = STD_S for standard normal cdf
double normalcdf(double x, double mu, double s) {
    assert(s >= 0);
    // normalize x
    x = (x - mu) / s;
    // if x >= 0, return the approximation
    if (x >= 0) {
	return 1 - 0.5 * pow(1 + A_1 * x + A_2 * pow(x, 2) + A_3 * pow(x, 3) + A_4 * pow(x, 4) + \
			     A_5 * pow(x, 5) + A_6 * pow(x, 6), -16);
    }
    // else change sign of negative x and return 1 - the approximation
    x = -1 * x;
    return 0.5 * pow(1 + A_1 * x + A_2 * pow(x, 2) + A_3 * pow(x, 3) + A_4 * pow(x, 4) + \
			 A_5 * pow(x, 5) + A_6 * pow(x, 6), -16);
}

// normal pdf function; use mu = STD_MU and s = STD_S for standard normal pdf
// note that M_PI is defined up to 16 digits of floating precision
double normalpdf(double x, double mu, double s) {
    assert(s >= 0);
    return pow(s * sqrt(2 * M_PI), -1) * exp(-1 * pow(x - mu, 2) / 2 / pow(s, 2));
}
