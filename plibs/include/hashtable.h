
#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include "list_internal.h"

typedef struct HashTable prv_hashtable_t;

prv_hashtable_t * prv_hashtable_new();

void prv_hashtable_delete(prv_hashtable_t * ht);

#define prv_hashtable_add(ht,key,value)  prv_hashtable_put(ht,key,value, NULL)

void * prv_hashtable_get(prv_hashtable_t * ht, char *key);

void prv_hashtable_remove(prv_hashtable_t *ht, char *key);

int prv_hashtable_put(prv_hashtable_t *ht, char *key, void * value, void(*free_value)(void*));


#endif
