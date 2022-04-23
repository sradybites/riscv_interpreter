#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "hashtable.h"

struct hashtable
{
    // TODO: define hashtable struct to use linkedlist as buckets
    linkedlist_t **buckets;
    int n_buckets;
};

/**
 * Hash function to hash a key into the range [0, max_range)
 */
static int hash(int key, int max_range)
{
    // TODO: feel free to write your own hash function (NOT REQUIRED)
    key = (key > 0) ? key : -key;
    return key % max_range;
}

hashtable_t *ht_init(int num_buckets)
{
    // TODO: create a new hashtable
    hashtable_t *ht = malloc(sizeof(hashtable_t));
    linkedlist_t **buckets_ptr = malloc(sizeof(linkedlist_t *) * num_buckets);
    for (int i = 0; i < num_buckets; i++) {
        buckets_ptr[i] = ll_init();
    }
    ht->buckets = buckets_ptr;
    ht->n_buckets = num_buckets;
    return ht;
}

void ht_free(hashtable_t *table)
{
    //TODO: free a hashtable from memory
    for (int i = 0; i < table->n_buckets; i++) {
        ll_free(table->buckets[i]);
    }
    free(table->buckets);
    free(table);
}

void ht_add(hashtable_t *table, int key, int value)
{
    // TODO: create a new mapping from key -> value.
    // If the key already exists, replace the value.
    int hashed = hash(key, table->n_buckets);
    linkedlist_t *buc = table->buckets[hashed];
    ll_add(buc, key, value);
}

int ht_get(hashtable_t *table, int key)
{
    // TODO: retrieve the value mapped to the given key.
    // If it does not exist, return 0
    int hashed = hash(key, table->n_buckets);
    linkedlist_t *buc = table->buckets[hashed];
    return ll_get(buc, key);
}

int ht_size(hashtable_t *table)
{
    // TODO: return the number of mappings in this hashtable
    int size = 0;
    for (int i = 0; i < table->n_buckets; i++) {
        size = size + ll_size(table->buckets[i]);
    }
    return size;
}
