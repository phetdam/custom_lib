# custom_lib

by Derek Huang

last updated on: 11-02-2018  
file created on: 08-29-2018

This directory will contain C source and header files that implement useful functions and data structures that are not part of the standard core C library. Credit is attributed as appropriate.

Most used for my own purposes; it'd be much easier to import these bad boys in than implement things from scratch over and over again. These also serve as a fun programming exercise for a beginner like me.

## Function/struct declarations

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

int hfunc(char *s, int siz);
void h_table_insert(h_table *ht, char *s);
int h_table_nsearch(h_table *ht, char *s);
void free_h_table(h_table *ht);
```

Todo: implement LCG, xorshift+ (128plus?)




