/**
 * d_array.c
 *
 * C implementation of what would be the equivalent of a Java ArrayList, except of
 * course C does not have generics so everything is based off of void * and pointer
 * typecasting. unfortunate. to avoid endianness issues, data access methods only
 * accept input of the same size as defined in the d_array; you will have to do
 * sign extensions manually if you wish to insert a smaller element into a d_array
 * with a larger element size.
 * source file that contains function definitions.
 *
 * sample usage:
 *
 * int n;
 * n = 10;
 * d_array *da = d_array__new(n, sizeof(int));
 * int i;
 * for (i = 0; i < da->siz; i++) {
 *     d_array__append(da, &i, sizeof(i));
 * }
 * for (; (i - da->siz) < da->siz; i++) {
 *     short int j;
 *     j = 444;
 *     int k;
 *     k = (int) j;
 *     d_array__insert(da, &k, sizeof(k), i);
 * }
 * d_array__free(da);
 *
 * Changelog:
 *
 * 11-07-2018
 *
 * removed ability to mix types. simpler to use, less room for error. 
 *
 * 11-04-2018
 *
 * fixed missing newline in one of the error messages, and updated sample usage
 *
 * 11-02-2018
 *
 * initial port from daqh.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "d_array.h"

// creates a new d_array; if DEFAULT_SIZ is given then starting size is 10 by default,
// while AUTO_SIZ will start the d_array from size 1, similar to Java's ArrayList.
// n is the starting number of elements in the array, and e is the max size of each
// element in the array (in bytes). n and e must be positive.
d_array *d_array__new(size_t n, size_t e) {
    // if n < 1, print error and exit
    if (n < 1) {
	fprintf(stderr, "%s: number of starting elements must be positive\n", D_ARRAY__NEW_N);
	exit(1);
    }
    // if e < 1, print error and exit
    if (e < 1) {
	fprintf(stderr, "%s: size of element must be positive\n", D_ARRAY__NEW_N);
	exit(1);
    }
    // create new d_array struct
    d_array *da = (d_array *) malloc(sizeof(d_array));
    // if da is NULL, print error and exit
    if (da == NULL) {
	fprintf(stderr, "%s: malloc error when allocating d_array\n", D_ARRAY__NEW_N);
	exit(2);
    }
    // new e array with n elements, currently uninitialized, element of size e
    da->a = malloc(n * e);
    // if a is NULL, print error and exit
    if (da->a == NULL) {
	fprintf(stderr, "%s: malloc error when allocating memory at %p->a\n",
		D_ARRAY__NEW_N, da);
	exit(2);
    }
    da->siz = 0;
    da->e_siz = e;
    da->max_siz = n;
    // return pointer
    return da;
}
// inserts an item e of size e_siz into the d_array struct at index i. one cannot insert
// to an index less than 0 or greater than da->siz - 1, and element e must have a size
// in bytes equal to da->e_siz for a legal insertion. cannot insert NULL.
// please do not try and mix types, for your own sanity. 
void d_array__insert(d_array *da, void *e, size_t e_siz, size_t i) {
    // if da is NULL, print error and exit
    if (da == NULL) {
	fprintf(stderr, "%s: cannot insert element into null d_array\n", D_ARRAY__INSERT_N);
	exit(1);
    }
    // if e is NULL, print error and exit
    if (e == NULL) {
	fprintf(stderr, "%s: cannot insert null element into d_array at %p\n",
		D_ARRAY__INSERT_N, da);
	exit(1);
    }
    // if e_siz != da->e_siz, print error and exit
    if (e_siz > da->e_siz) {
	fprintf(stderr, "%s: element at %p does not equal defined element size (%d) of "
		"d_array at %p\n", D_ARRAY__INSERT_N, e, da->e_siz, da);
	exit(1);
    }
    // if i > da->siz - 1, print error and exit (size_t is unsigned, so we do not have
    // to explicitly test for negative values thanks to two's complement
    if (i > da->siz - 1) {
	fprintf(stderr, "%s: cannot insert outside of defined array bounds of d_array at %p\n",
		D_ARRAY__INSERT_N, da);
	exit(1);
    }
    // insert element e at index i in da->a (i < da->siz)
    // counter, record size of da->a (da->siz)
    size_t c, n;
    n = da->siz;
    // if da->siz == da->max_siz, double array size
    if (n == da->max_siz) {
	da->max_siz *= 2;
	da->a = realloc(da->a, da->max_siz * e_siz);
	// if da->a is NULL, print error and exit
	if (da->a == NULL) {
	    fprintf(stderr, "%s: realloc failure inserting element at %p into d_array at %p\n",
		    D_ARRAY__INSERT_N, da, e);
	    exit(2);
	}
    }
    // we cannot do pointer arithmetic on void pointers, so we must first convert it to
    // char * so that we will be adding k bytes to it if we do for some initialized char
    // *p the operation *p += k
    // convert da->a to char *
    char *ca = (char *) da->a;
    // increment da->siz
    ++da->siz;
    // for each element i to da->siz - 1, shift them to the right by 1
    for (c = n - 1; c >= i; c--) {
	// copy element e_siz bytes at c to c + 1
	memcpy((c + 1) * e_siz + ca, c * e_siz + ca, e_siz);
    }
    // insert e_siz bytes from e at i
    memcpy(i * e_siz + ca, e, e_siz);
}
// for d_array da, appends an item size e_siz to da->a at index da->siz, and then increments
// da->siz. note that unlike d_array__insert(), an element can be added outside the bounds
// of da->a as when inserting, one can only insert in ranges 0 to da->siz - 1 inclusive.
// trying to mix types is not recommended.
void d_array__append(d_array *da, void *e, size_t e_siz) {
    // if da is NULL, print error and exit
    if (da == NULL) {
	fprintf(stderr, "%s: cannot append element onto null d_array\n", D_ARRAY__APPEND_N);
	exit(1);
    }
    // if e is NULL, print error and exit
    if (e == NULL) {
	fprintf(stderr, "%s: cannot append null element onto d_array at %p\n",
		D_ARRAY__APPEND_N, da);
	exit(1);
    }
    // if e_siz != da->e_siz, print error and exit
    if (e_siz != da->e_siz) {
	fprintf(stderr, "%s: element at %p does not equal defined element size (%d) of "
		"d_array at %p\n", D_ARRAY__APPEND_N, e, da->e_siz, da);
	exit(1);
    }
    // if da->siz == da->max_siz, double array size
    if (da->siz == da->max_siz) {
	da->max_siz *= 2;
	da->a = realloc(da->a, da->max_siz * da->e_siz);
	// if da->a is NULL, print error and exit
	if (da->a == NULL) {
	    fprintf(stderr, "%s: realloc failure appending element at %p onto d_array at %p\n",
		    D_ARRAY__APPEND_N, da, e);
	    exit(2);
	}
    }
    // convert da->a to char *
    char *ca = (char *) da->a;
    // increment da->siz
    ++da->siz;
    // append element at index da->siz - 1 (we incremented already)
    memcpy((da->siz - 1) * da->e_siz + ca, e, e_siz);
}









// frees a d_array struct
void d_array__free(d_array *da) {
    // if da or da->a is NULL, print error and exit
    if (da == NULL || da->a == NULL) {
	fprintf(stderr, "%s: cannot free null pointer\n", D_ARRAY__FREE_N);
	exit(1);
    }
    free(da->a);
    free(da);
}
