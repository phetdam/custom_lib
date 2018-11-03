/**
 * d_array.c
 *
 * C implementation of what would be the equivalent of a Java ArrayList, except of
 * course C does not have generics so everything is based off of void * and pointer
 * typecasting. unfortunate. please cast pointer appropriately before dereferencing,
 * although the design of d_array makes it possible to mix types within the array 
 * as long as they are less than or equal to the specified element size, it is not
 * recommended to abuse the d_array design in this manner.
 * source file that contains function definitions.
 *
 * sample usage:
 *
 * typedef long long ll;
 * d_array *da = d_array__new(10, sizeof(ll));
 * int i;
 * for (i = 0; i < da->siz; i++) {
 *     *((11 *) da->a + i) = i;
 * }
 * for (; (i - da->siz) < da->siz; i++) {
 *     
 * }
 * 
 *
 * Changelog:
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
// in bytes equal to or less than da->e_siz for a legal insertion. cannot insert NULL.
// note that this function allows type mixing in the d_array, but for your sanity, this
// is probably not a good idea for you to do unless you have a VERY good reason.
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
    // if e_siz > da->e_siz, print error and exit
    if (e_siz > da->e_siz) {
	fprintf(stderr, "%s: element at %p too large to insert into d_array at %p",
		D_ARRAY__INSERT_N, e, da);
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
    // counter, record size of da->a (da->siz) and da->e_siz
    size_t c, n, e_siz_max;
    n = da->siz;
    e_siz_max = da->e_siz;
    // if da->siz == da->max_siz, double array size
    if (n == da->max_siz) {
	da->max_siz *= 2;
	da->a = realloc(da->a, da->max_siz * e_siz_max);
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
	// copy element e_siz_max bytes at c to c + 1
	memcpy((c + 1) * e_siz_max + ca, c * e_siz_max + ca, e_siz_max);
    }
    // insert e_siz bytes from e  at i
    memcpy(i * e_siz_max + ca, e, e_siz);
}
// for d_array da, appends an item size e_siz to da->a at index da->siz, and then increments
// da->siz. note that unlike d_array__insert(), an element can be added outside the bounds
// of da->a as when inserting, one can only insert in ranges 0 to da->siz - 1 inclusive. like
// d_array__insert(), also allows type mixing, although not recommended.
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
    // if e_siz > da->e_siz, print error and exit
    if (e_siz > da->e_siz) {
	fprintf(stderr, "%s: element at %p too large to append to d_array at %p",
		D_ARRAY__APPEND_N, e, da);
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
