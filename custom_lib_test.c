/**
 * custom_lib_test.c
 *
 * quick program to test whatever features are being written for custom_lib.
 *
 * Changelog:
 *
 * 11-16-2018
 *
 * added code in test area to reflect changes in sample code of d_array.c
 *
 * 11-10-2018
 *
 * test area updated to sample code given in d_array.c; updated #include of header 
 * file to be more modular
 *
 * 11-04-2018
 *
 * fixed some spelling errors, and double-checked the type-mixing demo
 *
 * 11-03-2018
 *
 * name of file changed to custom_lib_test.c, with additional macros for help string,
 * program name, current package being tested, help flag and string added. main body 
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

// current package being tested (update as necessary with correct header file)
#define CUR_TEST "d_array.h"
#include CUR_TEST

// program name
#define PROGNAME "custom_lib_test"
// help flag
#define HELP_FLAG "--help"

// help string
#define HELP_STR "Usage: " PROGNAME " [ " HELP_FLAG " ]\n" \
    "driver program to test any features in custom_lib.\n\n" \
    "current package being tested: " CUR_TEST "\b\b  "
// two backspaces move the cursor back before writing whitespace to cover the .h

// driver program to test any features in the custom_lib
int main(int argc, char **argv) {
    // run normally if there are no arguments
    /*** TESTING AREA ***/
    if (argc == 1) {
	int n;
	n = 10;
	// memory taken is 4 * n bytes, with 0 elements (da->siz is  0)
	d_array *da = d_array__new(n, sizeof(int));
	int i;
	// append n items
	for (i = 0; i < n; i++) {
	    d_array__append(da, &i);
	}
	int j;
	j = 444;
	// insert n items at last index
	for (i = 0; i < n; i++) {
	    d_array__insert(da, &j, n - 1);
	}
	// print results using d_array__get()
	for (i = 0; i < da->siz; i++) {
	    printf("%d", *((int *) d_array__get(da, i)));
	    if (i < da->siz - 1) { printf(" "); }
	}
	printf("\n");
	// remove n elements from index n / 2
	for (i = 0; i < n; i++) {
	    d_array__remove(da, n / 2);
	}
	// print results using d_array__getcpy()
	for (i = 0; i < da->siz; i++) {
	    d_array__getcpy(&j, da, i);
	    printf("%d", j);
	    if (i < da->siz - 1) { printf(" "); }
	}
	printf("\n");
	// set j to 7
        j = 7;
	// set every other index to some number from rand()
	for (i = 0; i < n; i = i + 2) {
	    d_array__set(da, i, &j);
	}
	// print results using d_array__get()
	for (i = 0; i < n; i++) {
	    printf("%d", *((int *) d_array__get(da, i)));
	    if (i < da->siz - 1) { printf(" "); }
	}
	printf("\n");
	// free memory
	d_array__free(da);
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
