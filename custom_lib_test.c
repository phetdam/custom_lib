/**
 * custom_lib_test.c
 *
 * quick program to test whatever features are being written for custom_lib.
 *
 * Changelog:
 *
 * 11-03-2018
 *
 * name of file changed to custom_lib_test.c, with additional macros for help string,
 * program name, curretn package being tested, help flag and string added. main body 
 * rewritten to test d_array package and to incorporate macros.
 *
 * ??-??-2018
 *
 * not entirely sure when this was created; originally titled main.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
// any other header files to include
//#include "stats.h"
#include "d_array.h"
// program name
#define PROGNAME "custom_lib_test"
// help flag
#define HELP_FLAG "--help"
// current package being tested (update as necessary)
#define CUR_TEST "d_array"
// help string
#define HELP_STR "Usage: " PROGNAME " [ " HELP_FLAG " ]\n"\
    "drive program to test any features in custom_lib.\n\n"\
    "current package being tested: " CUR_TEST

// driver program to test any features in the custom_lib
int main(int argc, char **argv) {
    argv[0] = "main.exe";
    // run normally if there are no arguments
    /*** TESTING AREA ***/
    if (argc == 1) {

    }
    // else if there is one argument
    else if (argc == 2) {
	// if it is the help string
	if (strcmp(argv[1], HELP_FLAG) == 0) {
	    printf("%s\n", HELP_STR);
	    return 0;
	}
	// else unrecognized; print error and exit
	fprintf(stderr, "%s: unknown flag \'%s\'. please type \'%s %s\' for usage.\n",
		PROGNAME, argv[1], PROGNAME, HELP_FLAG);
	return 0;
    }
    // else too many arguments; print error and exit
    else {
	printf("%s: too many arguments. type \'%s %s\' for usage.\n", PROGNAME, PROGNAME, HELP_FLAG);
    }
    return 0;
}
