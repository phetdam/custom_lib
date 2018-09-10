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

	// x, start and step
	double x, m, step;
	m = -3;
	step = 0.1;
	x = m - step;
	// standardized mu and s
	double mu = 0;
	double s = 1;
	// print headers
	printf("        %15s         %15s\n", "normalcdf()", "1-normalcdf()");
	// while x is less than or equal to mean
	while ((x += step) <= mu + step) {
	    printf("%+7.3lf %15.9lf %+7.3lf %15.9lf\n", x, normalcdf(x, mu, s), -1 * x, 1 - \
		   normalcdf(-1 * x, mu, s));
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
