/**
 * d_array.c
 *
 * C implementation of what would be the equivalent of a Java ArrayList, except of
 * course C does not have generics so everything is based off of void * and pointer
 * typecasting. unfortunate. to avoid endianness issues, data access methods only
 * accept input of the same size as defined in the d_array; you will have to do
 * sign extensions manually if you wish to insert a smaller element into a d_array
 * with a larger element size. contains a generic framework for printing elements
 * of the d_array of varying types (including user-defined types, although that
 * requires the user to create their own function to convert a single element
 * to a string).
 *
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
 * 11-23-2018
 *
 * removed problem with using d_array__tostr on a char * array; was due to accidental
 * casting of void * in __tostr_el__char__ptr to char * instead of returning a 
 * dereferenced char **. modified free() to work properly with d_arrays that are
 * of a pointer type; will also free the memory pointed to by each element.
 *
 * 11-21-2018
 *
 * it's midnight (0000)! modified the insert function to prevent infinite loop when
 * inserting at index 0 due to unsigned integer overflow. added implementations of
 * int and char versions of the __tostr_el__* family of functions. updated file info.
 * added implementation of d_array__tostr, modified signature of d_array__new (i
 * added 4 new parameters). also it is now 0500+, not 0000. at 1311: added if
 * statement to check if char * returned by __tostr_el is NULL and handle error.
 * changed s_max to BUFSIZ in d_array__tostr, and removed debug comments from 
 * __tostr_el__int. added if statement to check if __tostr_el is NULL. prefixed the
 * separating, pre, and post element chars with "__" to imply that they are not
 * for the user to touch.
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

// writes an integer element of a d_array to a string, and returns char *
// returns NULL in case of error
char *__tostr_el__int(const void *e) {
    // integer at e, length of int, copy of int at e
    int k, l_k, c_k;
    k = *((int *) e);
    l_k = 0;
    c_k = k;
    // special cases:
    // if k == 0, increment l_k (loop will not be triggered)
    if (k == 0) {
	l_k++;
    }
    // else if integer is negative, increment l_k and eliminate sign of c_k
    else if (k < 0) {
	l_k++;
	c_k = ~c_k + 0x1;
    }
    // get length of integer
    while (c_k > 0) {
	c_k = c_k / 10;
	l_k++;
    }
    // create string of length l_k
    char *s;
    s = (char *) malloc(l_k + 1);
    // if s is NULL, print error and exit
    if (s == NULL) {
	fprintf(stderr, "%s: malloc failure for element at %p\n", __TOSTR_EL__INT_N, e);
	exit(2);
    }
    // use sprintf to write int to s; if no. characters returned <= 0,
    // return NULL
    if (sprintf(s, "%d", k) <= 0) {
	return NULL;
    }
    // write null-terminating character and return
    *(s + l_k) = '\0';
    return s;
}
// simply returns e cast to char *
char *__tostr_el__char(const void *e) {
    return (char *) e;
}
// simply returns dereferenced e cast to char ** (characters already in heap)
char *__tostr_el__char__ptr(const void *e) {
    return *((char **) e);
}
char *__tostr_el__long(const void *e) {
    return NULL;
}
char *__tostr_el__double(const void *e) {
    return NULL;
}

// given a d_array * da, size_t si and ei, will write elements si to ei - 1 in string form
// according to da's __tostr_el function, element separator sep, pre- and post-char pp, and
// return a char * to that string.
// must free() string later or else memory will leak. can use ALL__(d_array *da) to write
// all the elements of da to the string that d_array__tostr will return a char * to.
// recommended to use a sep/pp format specifing macro for both default and user-defined types.
// ex. d_array__tostr(da, 0, da->siz) -> d_array__tostr(ALL__(da))
char *d_array__tostr(d_array *da, size_t si, size_t ei) {
    // if da is NULL, print error and exit
    if (da == NULL) {
	fprintf(stderr, "%s: cannot convert null d_array to string\n", D_ARRAY__TOSTR_N);
	exit(1);
    }
    // if si > da->siz - 1, or ei > da->siz, print error and exit
    if (si > da->siz - 1 || ei > da->siz) {
	fprintf(stderr, "%s: cannot return elements outside of array bounds of d_array at %p\n",
		D_ARRAY__TOSTR_N, da);
	exit(1);
    }
    // function pointer for da->__tostr_el
    char *(*__tostr_el)(const void *);
    __tostr_el = da->__tostr_el;
    // if NULL, we do not have the required function to convert each element to string,
    // so we can only print error and exit
    if (__tostr_el == NULL) {
	fprintf(stderr, "%s: d_array at %p does not contain valid __tostr_el function\n",
		D_ARRAY__TOSTR_N, da);
	exit(1);
    }
    // char * for string, da->a cast as char *, char * to individual element string
    char *s, *ca, *ce;
    // max capacity of string, current pointer offset, length of string at ce
    // current pointer offset no. bytes s needs to be offset for new write
    size_t s_max, s_o, s_ce;
    // offset for width of sep, pr_c, or ps_c
    int s_spo;
    s_spo = 0;
    // start at BUFSIZ, trade memory for speed
    s_max = BUFSIZ;
    // offset at 0
    s_o = 0;
    // allocate space for buffer
    s = (char *) malloc(s_max);
    // cast da->a to char *
    ca = (char *) da->a;
    // if da->__pr_c != '\0', write it to s and increment s_o
    if (da->__pr_c != '\0') {
	*s = da->__pr_c;
	s_o++;
    }
    // using si as counter, while si < ei
    while (si < ei) {
	// get char * to string version of element at si
	ce = da->__tostr_el((void *) (ca + si * da->e_siz));
	// if ce is NULL, print error and exit
	if (ce == NULL) {
	    fprintf(stderr, "%s: write error with element at %p in d_array at %p\n",
		    D_ARRAY__TOSTR_N, ca + si * da->e_siz, da);
	    exit(2);
	}
	// get length of ce; set to 1 if da->__tostr_el == __tostr_el__char
	// optimization only for characters
	if (da->__tostr_el == __tostr_el__char) {
	    s_ce = 1;
	}
	else {
	    s_ce = strlen(ce);
	}
	// if __sep is not '\0' and si < ei - 1, or __ps_c is not '\0' and si == ei - 1,
	// then set s_spo to 1
	if ((da->__sep != '\0' && si < ei - 1) ||
	    (si == ei - 1 && da->__ps_c != '\0')) { s_spo = 1; }
	// if s_max - s_o - s_spo - 1 (null char) < s_ce, not enough space: realloc
	// we use a while loop because in case of an arbitrary length string, it can be
	// much longer than the s_max, and we need the additional or condition
	// s_o >= s_max - s_spo - 1 because when s_ce == 1, and s_max - s_o == 1, if
	// s_spo == 1, then lhs will be negative and there will be unsigned overflow.
	// the condition s_o >= s_max - s_spo - 1 is sufficient.
	while (s_o >= s_max - 2 || s_max - s_o - s_spo - 1 < s_ce) {
	    // double size of s_max
	    s_max = 2 * s_max;
	    // realloc
	    s = (char *) realloc((void *) s, s_max);
	    // if s is NULL, print error and exit
	    if (s == NULL) {
		fprintf(stderr, "%s: realloc error managing memory for d_array at %p\n",
			D_ARRAY__TOSTR_N, da);
		exit(2);
	    }
	}
	// write s_ce characters at s + s_o, and add s_ce to s_o
	strncpy(s + s_o, ce, s_ce);
	s_o = s_o + s_ce;
	// if s_spo != 0
	if (s_spo != 0) {
	    // if __sep != '\0' and si < ei - 1, write __sep
	    if (da->__sep != '\0' && si < ei - 1) {
		*(s + s_o) = da->__sep;
	    }
	    // else if __ps_c != '\0;' and si == ei - 1, write __ps_c
	    else if (da->__ps_c != '\0' && si == ei - 1) {
		*(s + s_o) = da->__ps_c;
	    }
	    // reset s_spo to 0
	    s_spo = 0;
	    // increment s_o
	    s_o++;
	}
	// if address of da->__tostr_el != address of __tostr_el__char, free ce
	if (da->__tostr_el != __tostr_el__char) {
	    free(ce);
	}
	// increment si
	si++;
    }
    // set s + s_o to '\0'
    *(s + s_o) = '\0';
    // realloc to size s_o + 1
    s = (char *) realloc((void *) s, s_o + 1);
    // return s
    return s;
}
// creates a new d_array; if DEFAULT_SIZ is given then number of elements before resize
// is 10 by default, while with AUTO_SIZ the number will be 1, similar to Java's ArrayList.
// n is the no. elements that can be added before a resize is needed, e is the size of each
// element in the array (in bytes), __tef is a function that returns an element of the
// d_array as a string, *__t is a char * to a string literal determining the type of da,
// sep is the character printed between elements, __pr_c is the char printed before the first
// element, and __ps_c is the char printed after the last element returned by d_array__tostr.
//
// n and e must be positive, and __tef is optional, although then the d_array__tostr method
// cannot be used on the d_array that has a NULL __tef (ex. void *).
// note: only call like d_array__new(some_siz, sizeof(your_type), __special_tostr_el) only
// for user-defined types. default implementations are provided and can be accessed through
// macros as follows: ex. int, d_array__new(some_siz, D_ARRAY__INT)
// it is recommended that user-defined __tef and sep/pp be combined into a macro
d_array *d_array__new(size_t n, size_t e, char *(*__tef)(const void *), const char *__t,
		      char __sep, char __pr_c, char __ps_c) {
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
    // if __t is NULL, print error and exit
    if (__t == NULL) {
	fprintf(stderr, "%s: cannot pass NULL as a type\n", D_ARRAY__NEW_N);
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
    da->__tostr_el = __tef;
    da->t__ = (char *) __t;
    da->__sep = __sep;
    da->__pr_c = __pr_c;
    da->__ps_c = __ps_c;
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
    // we need the extra condition because c is unsigned
    for (c = n - 1; c >= i && c < n; c--) {
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
    char *ca;
    ca = (char *) da->a;
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
    // if the type of the d_array is a pointer type
    if (*(da->t__ + strlen(da->t__) - 1) == '*') {
	// free all of the memory pointed to by each pointer in da->a
	size_t i;
	i = 0;
	while (i < da->siz) {
	    free(da->a + i++);
	}
    }
    // free da->a and da
    free(da->a);
    free(da);
}
