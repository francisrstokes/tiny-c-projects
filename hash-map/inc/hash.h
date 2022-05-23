#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct hm_node_t {
    // Regular linked list stuff
    struct hm_node_t* next;

    // Store a pointer to the item data
    void* item;

    // Store the key so we can properly retrieve items
    char key[0];
} hm_node_t;

typedef struct {
    // Configurable number of buckets
    size_t numBuckets;
    // Pointer to a yet-to-be-created array of buckets
    hm_node_t** buckets;
} hashmap_t;

// The actual hashing function. This takes a string key, and the number of buckets
// to make life easier. The actual bucketing part could also happen outside of the
// hash function
size_t hash_function(const char* ptr, size_t buckets);

// Allocate and initialise a hashmap with a fixed bucketSize
hashmap_t* hm_create(const size_t buckets);

bool hm_remove(hashmap_t* hm, const char* key);
void* hm_get(hashmap_t* hm, const char* key);
void hm_set(hashmap_t* hm, const char* key, void* item);
void hm_free(hashmap_t* hm);

#endif
