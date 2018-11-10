/**
 * d_array.c
 *
 * C implementation of what would be the equivalent of a Java ArrayList, except of
 * course C does not have generics so everything is based off of void * and pointer
 * typecasting. unfortunate. to avoid endianness issues, data access methods only
 * accept input of the same size as defined in the d_array; you will have to do
 * sign extensions manually if you wish to insert a smaller element into a d_array
 * with a larger element size.
 * header file that contains declarations for functions, macros, and the struct.
 *
 * Changelog:
 *
 * 11-10-2018
 *
 * added macros and function declarations for d_array__get and d_array__getcpy. 
 * changed function declarations for d_array__insert and append. updated comments 
 * for declaration of d_array__new.
 *
 * 11-07-2018
 *
 * edited comments for the file and header to reflect removal of ability to mix types,
 * both for the sake of ease of use and to prevent endianness issues. added macro, 
 * comments, and function declaration for d_array__remove().
 *
 * 11-02-2018
 *
 * initial creation
 *
 */

#ifndef D_ARRAY_H
#define D_ARRAY_H
// include stddef.h for size_t
#include <stddef.h>
// automatic starting size (emulating Java's ArrayList) of 1
#define AUTO_SIZ 1
// default no. starting elements in dynamic array
#define DEFAULT_SIZ 10
// function names
#define D_ARRAY__NEW_N "d_array__new"
#define D_ARRAY__FREE_N "d_array__free"
#define D_ARRAY__INSERT_N "d_array__insert"
#define D_ARRAY__APPEND_N "d_array__append"
#define D_ARRAY__REMOVE_N "d_array__remove"
#define D_ARRAY__GET_N "d_array__get"
#define D_ARRAY__GETCPY_N "d_array__getcpy"
// struct for dynamic array
struct d_array {
    // point to an element (to serve as an array)
    void *a;
    // size of each element in the d_array, no. elements in d_array, no. elements
    // d_array can hold (before resizing)
    size_t e_siz, siz, max_siz;
};
typedef struct d_array d_array;
// creates a new d_array; if DEFAULT_SIZ is given then number of elements before resize
// is 10 by default, while with AUTO_SIZ the number will be 1, similar to Java's ArrayList.
// n is the no. elements tha can be added before a resize is needed, e is the size of each
// element in the array (in bytes). n and e must be positive.
d_array *d_array__new(size_t n, size_t e);
// inserts an item e of size da->e_siz into the d_array struct at index i. one cannot
// insert to an index less than 0 or greater than da->siz - 1. cannot insert NULL.
// please do not try and mix types, for your own sanity. 
void d_array__insert(d_array *da, void *e, size_t i);
// for d_array da, appends an item size da->e_siz to da->a at index da->siz, and then
// increments da->siz. trying to mix types is not recommended.
void d_array__append(d_array *da, void *e);
// for d_array da, removes an item at index i, where i >= 0 and i < da->siz. da->siz will be
// decremented, and all elements shifted as appropriate to fill in the gaps. if da->siz is 0,
// an attempt to remove an element will cause and error and halt execution. does not zero
// the former last element of the d_array upon removal of an element.
void d_array__remove(d_array *da, size_t i);
// returns a void * to the element in d_array da located at index i
void *d_array__get(d_array *da, size_t i);
// for an element located at address p, for d_array da, the ith element of da will be
// written directly to the address at p.
void d_array__getcpy(void *p, d_array *da, size_t i);
// frees a d_array struct
void d_array__free(d_array *da);

#endif /* D_ARRAY_H */
