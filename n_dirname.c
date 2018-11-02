/**
 * n_dirname.c
 *
 * my own version of dirname. given a file name, it removes the 
 * trailing element and '/' to return the directory name of the file.
 * however, if there is a mix of '/' and '\', it will return a conflict,
 * and will automatically try to determine if the path given is a windows
 * path or a unix path. the "-nt" flag will suppress this behavior and have 
 * n_dirname behave like the classic dirname, which looks only for '/'.
 *
 * Changelog:
 *
 * 10-15-2018
 *
 * get_dir_name() macros added, specification added
 *
 * 10-14-2018
 *
 * changelog added, file comments added
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// help flag
#define HELP_FLAG "--help"
// no test flag
#define NT_FLAG "-nt"
// program name
#define PROGNAME "n_dirname"
// get_dir_name macros:
// try to resolve windows/unix pathnames
#define AUTO 0
// act like old dirname; care only about '/'
#define CLSC 1

/**
 * takes a string, assumed to be a filename with directory path, and an 
 * unsigned int that determines whether it should act like classic dirname,
 * or whether it should attempt to resolve windows/unix pathnames.
 * 
 * int macros:
 *
 * AUTO 0 - try to resolve windows/unix pathnames
 * CLSC 1 - act like old dirname; care only about '/'
 *
 * POSIX specifications:
 *
 * 1. If string is //, skip steps 2 to 5.
 * 2. If string consists entirely of <slash> characters, string shall be set 
 * to a single <slash> character. In this case, skip steps 3 to 8.
 * 3. If there are any trailing <slash> characters in string, they shall be removed.
 * 4. If there are no <slash> characters remaining in string, string shall be set 
 * to a single <period> character. In this case, skip steps 5 to 8.
 * 5. If there are any trailing non- <slash> characters in string, they shall be removed.
 * 6. If the remaining string is //, it is implementation-defined whether steps 7 
 * and 8 are skipped or processed.
 * 7. If there are any trailing <slash> characters in string, they shall be removed.
 * 8. If the remaining string is empty, string shall be set to a single <slash> character.
 * 
 * The dirname utility shall write a line to the standard output in the following format:
 * "%s\n", <resulting string>
 */
char *get_dir_name(char *p, unsigned int f) {
    // if AUTO, operate normalle
    if (f == AUTO) {
	;
    }
    // else if CLSC, act like old dirname
    else if (f == CLSC) {
	;
    }
    // else print error and exit
    else {
	fprintf(stderr, "%s: get_dir_name: fatal error: unknown uint flag %u.\n", PROGNAME, F);
	exit(2);
    }
    return NULL;
}


int main(int argc, char **argv) {
    // char *
    char *dn;
    // if there is only one argument, run normally
    if (argc == 2) {
	// if the string is the help flag, print usage and exit
	if (strcmp(argv[1], HELP_FLAG) == 0) {
	    printf("Usage: %s [ [ -nt ] file ]\n       %s [ --help ]\n"
		   "more stuff coming soon! acts like dirname\n", PROGNAME, PROGNAME);
	    return 0;
	}
	// else if the argument is NT_FLAG
	else if (strcmp(argv[1], NT_FLAG) == 0) {
	    fprintf(stderr, "%s: error: missing argument for %s flag. type \'%s \'%s "
		    "for usage.\n", PROGNAME, NT_FLAG, PROGNAME, HELP_FLAG);
	    return 1;
	}
	// else execute main logic
	else {
	    dn = get_dir_name(argv[1], AUTO);
	}
    }
    // else if there are two arguments
    else if (argc == 3) {
	// if the flag is NT_FLAG
	if (strcmp(argv[1], NT_FLAG) == 0) {
	    // execute main logic, simulating dirname
	    dn = get_dir_name(argv[2], CLSC);
	}
	// else if the string is the help flag, print error and exit
	else if (strcmp(argv[1], HELP_FLAG) == 0) {
	    fprintf(stderr, "%s: error: cannot use %s with an additional argument.\n", \
		    PROGNAME, argv[1]);
	    return 1;
	}
	// else print error and exit
	else {
	    fprintf(stderr, "%s: error: unknown flag \'%s\'. type \'%s %s\' for usage.\n", \
		    PROGNAME, argv[1], PROGNAME, HELP_FLAG);
	    return 1;
	}
    }
    // else print error and exit
    else {
	// if there are no arguments
	if (argc == 1) {
	    fprintf(stderr, "%s: error: no arguments. type \'%s %s\' for usage.\n", \
		    PROGNAME, PROGNAME, HELP_FLAG);
	}
	// else
	else {
	    fprintf(stderr, "%s: error: too many arguments. type \'%s %s\' for usage.\n", \
		    PROGNAME, PROGNAME, HELP_FLAG);
	}
	// return 1
	return 1;
    }
    // print dir name and newline
    printf("%s\n", dn);
    // exit
    return 0;
}
