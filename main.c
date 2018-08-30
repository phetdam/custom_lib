#include <stdio.h>
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
    double x = 1.1;
    double mu = 0;
    double s = 1;
    printf("normalpdf(%.2lf, %.2lf, %.2lf) = %.9lf\n", x, mu, s, normalpdf(x, mu, s));
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
