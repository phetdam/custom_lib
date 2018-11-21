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
 * header file that contains declarations for functions, macros, and the struct.
 *
 * Changelog:
 *
 * 11-21-2018
 *
 * added macros and function declarations for __tostr_el__* functions, as well as the
 * function declaration for d_array__tostr. added ALL__ macro for d_array__tostr
 * to simplify notation for returning entire array as string without varargs, and added
 * macros for names of __tostr_el__* function and d_array__tostr. added __tostr_el
 * function pointer to d_array struct and three chars, one to be printed in between
 * elements, and the other two to be printed at the start and end of printed elements
 * respectively. added macros to specify __tostr_el__* function and sep/pr_c/ps_c
 * format to simplify the usage of d_array__new, which got 4 new parameters. added
 * macro format for void *
 *
 * 11-16-2018
 *
 * updated comments so that they reflect the comments in d_array.c, and added the
 * function declaration for d_array__set. added function name macro for set.
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
// user function names
#define D_ARRAY__NEW_N "d_array__new"
#define D_ARRAY__FREE_N "d_array__free"
#define D_ARRAY__INSERT_N "d_array__insert"
#define D_ARRAY__APPEND_N "d_array__append"
#define D_ARRAY__REMOVE_N "d_array__remove"
#define D_ARRAY__GET_N "d_array__get"
#define D_ARRAY__GETCPY_N "d_array__getcpy"
#define D_ARRAY__SET_N "d_array__set"
#define D_ARRAY__TOSTR_N "d_array__tostr"
// struct for dynamic array
struct d_array {
    // point to an element (to serve as an array)
    void *a;
    // size of each element in the d_array, no. elements in d_array, no. elements
    // d_array can hold (before resizing)
    size_t e_siz, siz, max_siz;
    /**
     * internal function pointer that the d_array will use to write an element
     * when d_array__write is called. __tostr_el requires the following parameters
     *
     * const void *  const void * to an element in corresponding d_array
     *
     * proper casting and string formatting is left to the user for user-defined types.
     * it is recommended to use sprintf or snprintf for this purpose.
     * d_array package includes predefined functions for int, char, long, double, char *
     * (string). more implementations may be added later
     *
     * returns char * to element as string, or NULL upon error
     */
    char *(*__tostr_el)(const void *);
    // char element separator, char printed before elements, char printed after elements
    char sep, pr_c, ps_c;
};
typedef struct d_array d_array;
// declarations for default functions to pass as a d_array's __tostr_el function
char *__tostr_el__int(const void *e);
char *__tostr_el__char(const void *e);
char *__tostr_el__long(const void *e);
char *__tostr_el__double(const void *e);
char *__tostr_el__string(const void *e);
// string names for __tostr_el__* functions
#define __TOSTR_EL__INT_N "__tostr_el__int"
#define __TOSTR_EL__CHAR_N "__tostr_el__char"
#define __TOSTR_EL__LONG_N "__tostr_el__long"
#define __TOSTR_EL__DOUBLE_N "__tostr_el__double"
#define __TOSTR_EL__STRING_N "__tostr_el__string"
// macros for defining d_array type, appropriate pointer to bind to __tostr_el, and
// correct format for separators and pre- + post- char
#define D_ARRAY__INT sizeof(int), __tostr_el__int, ' ', '[', ']'
#define D_ARRAY__CHAR sizeof(char), __tostr_el__char, '\0', '\0', '\0'
#define D_ARRAY__LONG sizeof(long), __tostr_el__long, ' ', '[', ']'
#define D_ARRAY__DOUBLE sizeof(double), __tostr_el__double, ' ', '[', ']'
#define D_ARRAY__STRING sizeof(char *), __tostr_el__string, ',', '[', ']'
#define D_ARRAY__VOIDPTR sizeof(void *), NULL, '\0', '\0', '\0'
// macro to replace all 3 arguments of d_array__tostr so that all elements in the array
// will be written into the string that d_array__tostr will return a char * to
#define ALL__(_DA) _DA, 0, _DA->siz
// given a d_array * da, size_t si and ei, will write elements si to ei - 1 in string form
// according to da's __tostr_el function, element separator sep, pre- and post-char pp, and
// return a char * to that string.
// must free() string later or else memory will leak. can use ALL__(d_array *da) to write
// all the elements of da to the string that d_array__tostr will return a char * to.
// recommended to use a sep/pp format specifing macro for both default and user-defined types.
// ex. d_array__tostr(da, 0, da->siz) -> d_array__tostr(ALL__(da))
char *d_array__tostr(d_array *da, size_t si, size_t ei);
// creates a new d_array; if DEFAULT_SIZ is given then number of elements before resize
// is 10 by default, while with AUTO_SIZ the number will be 1, similar to Java's ArrayList.
// n is the no. elements that can be added before a resize is needed, e is the size of each
// element in the array (in bytes), __tef is a function that returns an element of the
// d_array as a string, sep is the character printed between elements, pr_c is the char
// printed before the first element, and ps_c is the char printed after the last element
// returned by d_array__toostr.
// n and e must be positive, and _tef is optional, although then the d_array__tostr method
// cannot be used on the d_array whose// is returned by d_array__new.
// note: only call like d_array__new(some_siz, sizeof(your_type), __special_tostr_el) only
// for user-defined types. default implementations are provided and can be accessed through
// macros as follows: ex. int, d_array__new(some_siz, D_ARRAY__INT)
// it is recommended that user-defined __tef and sep/pp be combined into a macro
d_array *d_array__new(size_t n, size_t e, char *(*__tef)(const void *), char sep, char pr_c, char ps_c);
// writes da->e_siz bytes from e into the d_array struct at index i, effectively inserting
// a new element into da. one cannot insert to an index less than 0 or greater than
// da->siz - 1, or insert NULL. please do not try and mix types, for your own sanity.
void d_array__insert(d_array *da, void *e, size_t i);
// for d_array da, appends an item to da->a at index da->siz, writing da->e_siz bytes from e.
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
// for an element located at address p, for the d_array da, da->e_siz bytes from p will
// overwrite the ith element in da.
void d_array__set(d_array *da, size_t i, void *p);
// frees a d_array struct
void d_array__free(d_array *da);

#endif /* D_ARRAY_H */
