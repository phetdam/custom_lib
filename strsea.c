/**
 * strsea.c
 *
 * main program that tests out the string hash table implemented in strh_table.c
 * reads input from stdin. solution to the "Sparse Arrays" HackerRank problem,
 * where one is given a list of strings of size n to put into a data structure 
 * and q string queries, and must output the number of times each queried string
 * can be found within the data struture. it is guaranteed that the number of
 * strings and queries are both 1000 at maximum and 1 at minimum. the length of
 * each string is 20 maximum, but the hash table supports strings of arbitrary
 * input length.
 *
 * the naive way would be to read all the strings into an array, or try to make
 * a set of strings and keep track of how many times they occurred, but searching
 * is still linear time. the hash table speeds the process up.
 *
 * the program here has been modified from the original solution to write results
 * to the output file strsea_out.
 *
 * recommended compilation: 'gcc -Wall -g -o strsea strsea.c strh_table.c strh_table.h'
 * please run by reading input file from stdin: './strsea < sparse_arrays_input01'
 *
 * Changelog:
 *
 * 09-19-2018
 *
 * copied original version from hackerrank; modified file output path and file 
 * output format
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strh_table.h"

// program name "string search"
#define PROGNAME "strsea"
// default name of output file (not included in hackerrank original version)
#define OUT_FILE "strsea_out"

int main(int argc, char **argv)
{
    FILE *fptr = fopen(OUT_FILE, "w");
    // get number of strings to insert into hash table, loop index
    int n, i;
    scanf("%d", &n);
    // new hash table of size H_SIZ
    h_table *ht = new_h_table(H_SIZ);
    // input buffer (BUFSIZ large enough for most small strings)
    char *buf = (char *) malloc(BUFSIZ * sizeof(char));
    // insert next n strings into hash table
    i = 0;
    while (i++ < n) {
        // read in string
        scanf("%s", buf);
        // insert into hash table
        h_table_insert(ht, buf);
        // clear buffer
        memset(buf, 0, strlen(buf) * sizeof(char));
    }
    // get number of query strings to search for
    scanf("%d", &n);
    // search for these strings
    i = 0;
    while (i++ < n) {
        // read in string
        scanf("%s", buf);
        // search for it in hash table and print it and its number of occurrences
	// in the hash table
        fprintf(fptr, "%d %s\n", h_table_nsearch(ht, buf), buf);
        // clear buffer
        memset(buf, 0, strlen(buf) * sizeof(char));
    }
    // free hash table memory and close file
    free_h_table(ht);
    fclose(fptr);
    return 0;
}
