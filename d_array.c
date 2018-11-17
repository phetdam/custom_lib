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
 * for (i = 0; i < n; i++) {
 *     d_array__append(da, &i);
 * }
 * int j;
 * j = 444;
 * for (i = 0; i < n; i++) {
 *     d_array__insert(da, &j, n - 1);
 * }
 * for (i = 0; i < da->siz; i++) {
 *     printf("%d", *((int *) d_array__get(da, i)));
 *     if (i < da->siz - 1) { printf(" "); }
 * }
 * printf("\n");
 * for (i = 0; i < n; i++) {
 *     d_array__remove(da, n / 2);
 * }
 * for (i = 0; i < da->siz; i++) {
 *     d_array__getcpy(&j, da, i);
 *     printf("%d", j);
 *     if (i < da->siz - 1) { printf(" "); }
 * }
 * printf("\n");
 * j = 7;
 * for (i = 0; i < n; i = i + 2) {
 *     d_array__set(da, i, &j);
 * }
 * for (i = 0; i < n; i++) {
 *     printf("%d", *((int *) d_array__get(da, i)));
 *     if (i < da->siz - 1) { printf(" "); }
 * }
 * printf("\n");
 * d_array__free(da);
 *
 * Changelog:
 *
 * 11-16-2018
 *
 * updated the comments for the insert and append functions so that they make 
 * more sense to the end user. added d_array__set, and updated sample usage 
 * to reflect the new function.
 *
 * 11-10-2018
 *
 * implemented "getter" functions, and changed the function signatures of 
 * d_array__insert and d_array__append to remove the size requirement (which is 
 * redundant; both the insert and append functions will attempt to read and write 
 * da->e_siz bytes). the burden of checking operand size before calling either
 * method is now left up to the programmer. updated sample usage, and corrected
 * problem with d_array__remove due to confusion over the index variable. updated
 * comments for d_array__new.
 *
 * 11-07-2018
 *
 * removed ability to mix types. simpler to use, less room for error. decided to work
 * on d_array__remove() instead of studying for exam. corrected accidental swapping
 * of pointer to element and pointer to d_array in realloc failure messages.
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

// creates a new d_array; if DEFAULT_SIZ is given then number of elements before resize
// is 10 by default, while with AUTO_SIZ the number will be 1, similar to Java's ArrayList.
// n is the no. elements tha can be added before a resize is needed, e is the size of each
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

// writes da->e_siz bytes from e into the d_array struct at index i, effectively inserting
// a new element into da. one cannot insert to an index less than 0 or greater than
// da->siz - 1, or insert NULL. please do not try and mix types, for your own sanity.
void d_array__insert(d_array *da, void *e, size_t i) {
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
    // if i > da->siz - 1, print error and exit (size_t is unsigned, so we do not have
    // to explicitly test for negative values thanks to two's complement
    if (i > da->siz - 1) {
	fprintf(stderr, "%s: cannot insert outside of defined array bounds of d_array at %p\n",
		D_ARRAY__INSERT_N, da);
	exit(1);
    }
    // insert element e at index i in da->a (i < da->siz)
    // counter, record size of da->a (da->siz), record size of element (da->e_siz)
    size_t c, n, e_siz;
    n = da->siz;
    e_siz = da->e_siz;
    // if da->siz == da->max_siz, double array size
    if (n == da->max_siz) {
	da->max_siz *= 2;
	da->a = realloc(da->a, da->max_siz * e_siz);
	// if da->a is NULL, print error and exit
	if (da->a == NULL) {
	    fprintf(stderr, "%s: realloc failure inserting element at %p into d_array at %p\n",
		    D_ARRAY__INSERT_N, e, da);
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

// for d_array da, appends an item to da->a at index da->siz, writing da->e_siz bytes from e.
// increments da->siz. trying to mix types is not recommended.
void d_array__append(d_array *da, void *e) {
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
    // record size of element (da->e_siz)
    size_t e_siz;
    e_siz = da->e_siz;
    // if da->siz == da->max_siz, double array size
    if (da->siz == da->max_siz) {
	da->max_siz *= 2;
	da->a = realloc(da->a, da->max_siz * e_siz);
	// if da->a is NULL, print error and exit
	if (da->a == NULL) {
	    fprintf(stderr, "%s: realloc failure appending element at %p onto d_array at %p\n",
		    D_ARRAY__APPEND_N, e, da);
	    exit(2);
	}
    }
    // convert da->a to char *
    char *ca = (char *) da->a;
    // increment da->siz
    ++da->siz;
    // append element at index da->siz - 1 (we incremented already)
    memcpy((da->siz - 1) * e_siz + ca, e, e_siz);
}

// for d_array da, removes an item at index i, where i >= 0 and i < da->siz. da->siz will be
// decremented, and all elements shifted as appropriate to fill in the gaps. if da->siz is 0,
// an attempt to remove an element will cause and error and halt execution. does not zero
// the former last element of the d_array upon removal of an element.
void d_array__remove(d_array *da, size_t i) {
    // if da is NULL, print error and exit
    if (da == NULL) {
	fprintf(stderr, "%s: cannot remove element from null d_array\n", D_ARRAY__REMOVE_N);
	exit(1);
    }
    // if da->siz == 0, print error and exit
    if (da->siz == 0) {
	fprintf(stderr, "%s: cannot remove element from empty d_array at %p\n", D_ARRAY__REMOVE_N, da);
	exit(1);
    }
    // if i > da->siz - 1, print error and exit (size_t is unsigned, so we do not have
    // to explicitly test for negative values thanks to two's complement as da->siz - 1 >= 0)
    if (i > da->siz - 1) {
	fprintf(stderr, "%s: cannot remove element from outside defined array bounds of d_array at %p\n",
		D_ARRAY__REMOVE_N, da);
	exit(1);
    }
    // loop counter, size of element (da->e_siz)
    size_t c, e_siz;
    // convert da->a to char *
    char *ca = (char *) da->a;
    // get size of element
    e_siz = da->e_siz;
    // for all elements c to da->siz - 2, copy the next element c + 1 to c
    for (c = i; c < da->siz - 1; c++) {
	memcpy(c * e_siz + ca, (c + 1) * e_siz + ca, e_siz);
    }
    // decrement da->siz
    --da->siz;
    // if da->siz <= da->max_siz / 4, halve array size
    if (da->siz <= da->max_siz / 4) {
	da->max_siz /= 2;
	da->a = realloc(da->a, da->max_siz * e_siz);
	// if da->a is NULL, print error and exit
	if (da->a == NULL) {
	    fprintf(stderr, "%s: realloc failure halving size of d_array at %p\n",
		    D_ARRAY__APPEND_N, da);
	    exit(2);
	}
    }
}

// returns a void * to the element in d_array da located at index i
void *d_array__get(d_array *da, size_t i) {
    // if da is NULL, print error and exit
    if (da == NULL) {
	fprintf(stderr, "%s: cannot return void * to element %u of null d_array\n",
		D_ARRAY__GET_N, i);
	exit(1);
    }
    // if i > da->siz - 1, print error and exit (size_t is unsigned, so we do not have
    // to explicitly test for negative values thanks to two's complement
    if (i > da->siz - 1) {
	fprintf(stderr, "%s: cannot retrieve data outside of defined bounds of d_array at %p\n",
		D_ARRAY__GET_N, da);
	exit(1);
    }
    // cast da->a to char *
    char *ca;
    ca = (char *) da->a;
    // return address of element at i
    return (void *) (ca + i * da->e_siz);
}

// for an element located at address p, for d_array da, the ith element of da will be
// written directly to the address at p.
void d_array__getcpy(void *p, d_array *da, size_t i) {
    // if p is NULL, print error and exit
    if (p == NULL) {
	fprintf(stderr, "%s: cannot write to null address from d_array at %p\n",
		D_ARRAY__GETCPY_N, da);
	exit(1);
    }
    // if da is NULL, print error and exit
    if (da == NULL) {
	fprintf(stderr, "%s: cannot write to address %p from null d_array\n",
		D_ARRAY__GETCPY_N, p);
	exit(1);
    }
    // if i > da->siz - 1, print error and exit (size_t is unsigned, so we do not have
    // to explicitly test for negative values thanks to two's complement
    if (i > da->siz - 1) {
	fprintf(stderr, "%s: cannot retrieve data outside of defined bounds of d_array at %p\n",
		D_ARRAY__GETCPY_N, da);
	exit(1);
    }
    // cast da->a to char *
    char *ca;
    ca = (char *) da->a;
    // write da->e_siz bytes from address ca + i * da->e_siz to p
    memcpy(p, ca + i * da->e_siz, da->e_siz);
}

// for an element located at address p, for the d_array da, da->e_siz bytes from p will
// overwrite the ith element in da.
void d_array__set(d_array *da, size_t i, void *p) {
    // if da is NULL, print error and exit
    if (da == NULL) {
	fprintf(stderr, "%s: cannot write from address %p to null d_array\n",
		D_ARRAY__SET_N, p);
	exit(1);
    }
    // if p is NULL, print error and exit
    if (p == NULL) {
	fprintf(stderr, "%s: cannot write from null address to d_array at %p\n",
		D_ARRAY__SET_N, da);
	exit(1);
    }
    // if i > da->siz - 1, print error and exit (size_t is unsigned, so we do not have
    // to explicitly test for negative values thanks to two's complement
    if (i > da->siz - 1) {
	fprintf(stderr, "%s: cannot write outside of defined bounds of d_array at %p\n",
		D_ARRAY__GETCPY_N, da);
	exit(1);
    }
    // cast da->a to char *
    char *ca;
    ca = (char *) da->a;
    // write da->e_siz bytes to address ca + i * da->e_siz from p
    memcpy(ca + i * da->e_siz, p, da->e_siz);
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
