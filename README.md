# custom_lib

by Derek Huang

_last updated on: 11-07-2018_  
_file created on: 08-29-2018_

This directory will contain C source and header files that implement useful functions and data structures that are not part of the standard core C library. Credit is attributed as appropriate.

Most used for my own purposes; it'd be much easier to import these bad boys in than implement things from scratch over and over again. These also serve as a fun programming exercise for a beginner like me.

Also what I really hate about markdown: line breaks are created by putting __two spaces__ after a line of text. So it's very difficult to tell visually where the line break is...

## Function/struct declarations

##### d_array.c, d_array.h:

```c
struct d_array {
    void *a;
    size_t e_siz, siz, max_siz;
};
typedef struct d_array d_array;

d_array *d_array__new(size_t n, size_t e);
void d_array__insert(d_array *da, void *e, size_t e_siz, size_t i);
void d_array__append(d_array *da, void *e, size_t e_siz);
void d_array__remove(d_array *da, size_t i);
void d_array__free(d_array *da);
```

##### stats.c, stats.h:

```c
double normalcdf(double x, double mu, double s);
double normalpdf(double x, double mu, double s);
```

##### strh_table.c, strh_table.h:

```c
struct ht_node {
    char *str;
    struct ht_node *next;
};
typedef struct ht_node ht_node;

struct h_table {
    ht_node **table;
    int siz;
}
typedef struct h_table h_table;

h_table *new_h_table(int s);
int hfunc(char *s, int siz);
void h_table_insert(h_table *ht, char *s);
int h_table_nsearch(h_table *ht, char *s);
void free_h_table(h_table *ht);
```

Todo: implement LCG, xorshift+ (128plus?)




