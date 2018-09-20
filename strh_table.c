/**
 * strh_table.c
 *
 * source file for the functions declared in strh_table.h
 *
 * Changelog:
 *
 * 09-19-2018
 *
 * copied from solution to HackerRank Sparse Arrays problem; added change log
 *
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "strh_table.h"

// return a pointer to a new hash table of some size
h_table *new_h_table(int s) {
    assert(s > 0);
    // malloc hash table
    h_table *ht = (h_table *) malloc(sizeof(h_table));
    // malloc array of ht_node * and set them to NULL
    ht->table = (ht_node **) malloc(s * sizeof(ht_node *));
    memset(ht->table, 0, s * sizeof(ht_node *));
    // set size of hash table
    ht->siz = s;
    // return pointer
    return ht;
}
// hash function for the table; not platform portable
// generates integer hash for given string and table size
int hfunc(char *s, int siz) {
    // length of string, loop index, sum of all character int values
    int n, i, sum;
    n = strlen(s);
    assert(n > 0);
    // get sum of all characters in string
    sum = i = 0;
    while (i < n) {
        sum += *(s + i++);
    }
    // add by prime; mod by table size
    return (sum + 6691) % siz;
}
// insert a new string into the hash table
void h_table_insert(h_table *ht, char *s) {
    // get length of string
    int n = strlen(s);
    assert(n > 0);
    // create a new ht_node
    ht_node *htn = (ht_node *) malloc(sizeof(ht_node));
    // malloc string space needed for s and copy s to t
    char *t = (char *) malloc(n * sizeof(char) + 1);
    strncpy(t, s, n);
    // point htn->str to t, set next to NULL
    htn->str = t;
    htn->next = NULL;
    // calculate index of hash table to insert
    int ii = hfunc(s, ht->siz);
    // if *(ht->table + ii) is NULL, set it to htn and exit function
    if (*(ht->table + ii) == NULL) { 
        *(ht->table + ii) = htn;
        return;
    }
    // else create ht_node *; start at *(ht->table + ii)
    ht_node *hp = *(ht->table + ii);
    // loop until NULL is found (hp guaranteed not NULL)
    while (hp->next != NULL) {
        hp = hp->next;
    }
    // set next of ht_node to htn
    hp->next = htn;
}
// search for a string in the hash table; returns number of occurrences in the table
int h_table_nsearch(h_table *ht, char *s) {
    // length of string s, number of occurrences of s in tablbe
    int n, n_s;
    n = strlen(s);
    n_s = 0;
    assert(n > 0 && ht != NULL);
    // find index to search
    int i = hfunc(s, ht->siz);
    // pointer to ht_node * *(ht->table + i)
    ht_node *hp = *(ht->table + i);
    // while hp is not NULL
    while (hp != NULL) {
        // try to see if the string that hp->str points to is the same as s
        int res = strcmp(s, hp->str);
        // if res == 0 increment n_s
        if (res == 0) { n_s++; }
        // else advance up the linked list
        hp = hp->next;
    }
    // return n_s
    return n_s;
}
// free a hash table
void free_h_table(h_table *ht) {
    // pointers to current and next ht_node
    ht_node *hn_c, *hn_n;
    int i = 0;
    // for each list in the hash table
    while (i < ht->siz) {
        // set current and next
        hn_c = *(ht->table + i++);
        hn_n = (hn_c == NULL) ? NULL : hn_c->next;
        // while current (hn_c) is not null
        while (hn_c != NULL) {
            // free and update hn_c and hn_n
            free(hn_c->str);
            free(hn_c);
            hn_c = hn_n;
            hn_n = (hn_c == NULL) ? NULL : hn_c->next;
        }
    }
    // free ht->table and ht
    free(ht->table);
    free(ht);
}

