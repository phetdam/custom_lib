#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
// any other header files to include
#include "stats.h"

// driver program to test any features in the custom_lib
int main(int argc, char **argv) {
    argv[0] = "main.exe";
    // run normally if there are no arguments
    /*** TESTING AREA ***/
    if (argc == 1) {
	// why is the distribution for my old normalcdf using the erf not symmetric? weird
	// test using a bunch of values; normalcdf up to mean and then 1- normalcdf afterwards
	// compare values and see what's up

	// x, min-max range and step
	double x, min, max, step;
	min = -3;
	max = 3;
	step = 0.1;
	x = min - step;
	// standardized mu and s
	double mu = 0;
	double s = 1;
	// print headers
	printf("%13s %13s\n", "normalcdf()", "normalcdf1()");
	// while x is less than or equal to max
	while ((x += step) <= max) {
	    // old cdf and new cdf values; do 1 - * if x > mu
	    double oc, nc;
	    oc = normalcdf(x, mu, s);
	    oc = (x > mu) ? 1 - oc : oc;
	    nc = normalcdf1(x, mu, s);
	    nc = (x > mu) ? 1 - nc : nc;
	    printf("%13.9lf %13.9lf\n", normalcdf(x, mu, s), normalcdf1(x, mu, s));
	}
    }
    // else if there is one argument
    else if (argc == 2) {
	// if it is the help string
	if (strcmp(argv[1], "--help") == 0) {
	    printf("usage: %s [ --help ]\n"
		   "driver program to test any features in the custom_lib\n", argv[0]);
	    return 0;
	}
	// else unrecognized; print error and exit
	fprintf(stderr, "%s: unknown flag \'%s\'. please type \'%s --help\' for usage.\n", argv[0], \
		argv[1], argv[0]);
	return 0;
    }
    // else too many arguments; print error and exit
    else {
	printf("%s: too many arguments. type \'%s --help\' for usage.\n", argv[0], argv[0]);
    }
    return 0;
}
