/**
 * custom_lib_test.c
 *
 * quick program to test whatever features are being written for custom_lib.
 *
 * Changelog:
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
	// add ten elements
	int n = 10;
	// get d_array
	d_array *da = d_array__new(AUTO_SIZ, sizeof(int));
	// print (max) size of each element
	printf("size of element in d_array at %p: %u\n", da, da->e_siz);
	int i;
	// append n elements
	for (i = 0; i < n; i++) {
	    d_array__append(da, &i, sizeof(i));
	}
	// print all elements
	for (i = 0; i < n; i++) {
	    printf("%d ", *((int *) da->a + i));
	}
	printf("\n");
	// insert n + 4 elements, all are 0
	for (i = 1; i < n + 4; i++) {
	    int k = 444;
	    d_array__insert(da, &k, sizeof(k), i);
	}
	// print all elements after insert
	for (i = 0; i < da->siz; i++) {
	    printf("%d ", *((int *) da->a + i));
	}
	// mix type (4 bytes)
	char *s = "poo";
	// append string (plus null)
	d_array__append(da, s, strlen(s) + 1);
	// redefine as much bigger string (take advantage of fact that pointers are
	// 32 bits for mingw-pe32)
	s = "\"this is a really large string my guy\"";
	// append char * (4 bytes)
	d_array__append(da, &s, sizeof(s));
        // print original string and current string (through pointer)
	printf("%s %s\n", ((char *) da->a) + (da->siz - 2) * da->e_siz,
	       *((char **) ((char *) da->a + (da->siz - 1) * da->e_siz)));
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
