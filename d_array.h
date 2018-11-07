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
// struct for dynamic array
struct d_array {
    // point to an element (to serve as an array)
    void *a;
    // size of each element in the d_array, no. elements in d_array, no. elements
    // d_array can hold (before resizing)
    size_t e_siz, siz, max_siz;
};
typedef struct d_array d_array;
// creates a new d_array; if AUTO_SIZ is given then starting size is 20 by default
// n is the starting number of elements in the array, and e is the max size of each
// element in the array (in bytes). n and e must be positive.
d_array *d_array__new(size_t n, size_t e);
// inserts an item e of size e_siz into the d_array struct at index i. one cannot insert
// to an index less than 0 or greater than da->siz - 1, and element e must have a size
// in bytes equal to da->e_siz for a legal insertion. cannot insert NULL.
// please do not try and mix types, for your own sanity. 
void d_array__insert(d_array *da, void *e, size_t e_siz, size_t i);
// for d_array da, appends an item size e_siz to da->a at index da->siz, and then increments
// da->siz. note that unlike d_array__insert(), an element can be added outside the bounds
// of da->a as when inserting, one can only insert in ranges 0 to da->siz - 1 inclusive.
// trying to mix types is not recommended.
void d_array__append(d_array *da, void *e, size_t e_siz);
// for d_array da, removes an item at index i, where i >= 0 and i < da->siz. da->siz will be
// decremented, and all elements shifted as appropriate to fill in the gaps. if da->siz is 0,
// an attempt to remove an element will cause and error and halt execution. does not zero
// the former last element of the d_array upon removal of an element.
void d_array__remove(d_array *da, size_t i);
// frees a d_array struct
void d_array__free(d_array *da);

#endif /* D_ARRAY_H */
