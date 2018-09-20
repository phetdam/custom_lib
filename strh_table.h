/**
 * strh_table.h
 *
 * header file for hash table nodes, the table itself, and related functions
 *
 * Changelog:
 *
 * 09-19-2018
 * 
 * copied from solution to HackerRank Sparse Arrays problem; added change log
 *
 */

#ifndef STRH_TABLE_H
#define STRH_TABLE_H
// default hash table size
#define H_SIZ 512
// hash table node
struct ht_node {
    // pointer to string
    char *str;
    // pointer to next ht_node
    struct ht_node *next;
};
typedef struct ht_node ht_node;
// hash table
struct h_table {
    // ht_node ** (array of ht_node *)
    ht_node **table;
    // size of table (number of ht_node linked lists in the table)
    int siz;
};
typedef struct h_table h_table;
// return a pointer to a new hash table of some size
h_table *new_h_table(int s);
// hash function for the table; not platform portable (int width platform
// dependent). generates integer hash for given string and table size
int hfunc(char *s, int siz);
// insert a new string into the hash table
void h_table_insert(h_table *ht, char *s);
// search for a string in the hash table; returns occurrences of s in the table
int h_table_nsearch(h_table *ht, char *s);
// free a hash table
void free_h_table(h_table *ht);

#endif /* STRH_TABLE_H */
