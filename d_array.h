/**
 * d_array.c
 *
 * C implementation of what would be the equivalent of a Java ArrayList, except of
 * course C does not have generics so everything is based off of void * and pointer
 * typecasting. unfortunate. please cast pointer appropriately before dereferencing,
 * although the design of d_array makes it possible to mix types within the array 
 * as long as they are less than or equal to the specified element size, it is not
 * recommended to abuse the d_array design in this manner.
 * header file that contains declarations for functions, macros, and the struct.
 *
 * Changelog:
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
// inserts an item of size e into the d_array struct at index i. one cannot insert items
// to an index less than 0 or greater than da->siz - 1, and element e must have a size
// in bytes equal to or less than da->e_siz for a legal insertion. cannot insert NULL.
// note that this function allows type mixing in the d_array, but for your sanity, this
// is probably not a good idea for you to do unless you have a VERY good reason.
void d_array__insert(d_array *da, void *e, size_t i);
// for d_array da, appends an item to da->a at index da->siz, and then increments da->siz.
// note that unlike d_array__insert(), an element can be added outside the bounds of da->a
// as when inserting, one can only insert in ranges 0 to da->siz - 1 inclusive. like
// d_array__insert(), also allows type mixing, although not recommended.
void d_array__append(d_array *da, void *e);
// frees a d_array struct
void d_array__free(d_array *da);

#endif /* D_ARRAY_H */
