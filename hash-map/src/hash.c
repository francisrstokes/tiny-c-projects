#include "hash.h"
#include <malloc.h>
#include <string.h>

#ifndef HM_MAX_KEY_LENGTH
#define HM_MAX_KEY_LENGTH 512
#endif // HM_MAX_KEY_LENGTH

#define DJB2_INIT 5381
size_t hash_function(const char* ptr, const size_t buckets) {
    size_t hash = DJB2_INIT;

    uint8_t byte;
    while ((byte = *ptr++)) {
        hash = (size_t)(((hash << 5) + hash) + byte);
    }

    return hash % buckets;
}

hashmap_t* hm_create(const size_t buckets) {
    hashmap_t* hm = malloc(sizeof(hashmap_t));
    hm->numBuckets = buckets;
    hm->buckets = malloc(sizeof(hm_node_t*) * buckets);

    return hm;
}

void hm_set(hashmap_t* hm, const char* key, void* item) {
    // Get the hash of the key
    size_t hash = hash_function(key, hm->numBuckets);

    // Attempt to find the node
    hm_node_t* prevNode = NULL;
    hm_node_t* node = hm->buckets[hash];

    while (node) {
        if (strncmp(node->key, key, HM_MAX_KEY_LENGTH) == 0) {
            // Replace the value at this node
            node->item = item;
            return;
        }
        prevNode = node;
        node = node->next;
    }

    // We didn't find our key, malloc a new node and insert it
    size_t keyLength = strnlen(key, HM_MAX_KEY_LENGTH-1) + 1;
    hm_node_t* newNode = malloc(sizeof(hm_node_t) + keyLength);
    newNode->item = item;
    newNode->next = NULL;

    // Copy the key into the node directly
    strncpy(newNode->key, key, keyLength);
    newNode->key[keyLength] = '\0';

    // If there was a collision on the hash, we should have a pointer to the last node
    // which we can attach our new node to
    if (prevNode) {
        prevNode->next = newNode;
        return;
    }

    // Otherwise, we will simply insert this node at the hash position
    hm->buckets[hash] = newNode;
}

bool hm_remove(hashmap_t* hm, const char* key) {
    // Get the hash of the key
    size_t hash = hash_function(key, hm->numBuckets);

    // Attempt to find the node
    hm_node_t* prevNode = NULL;
    hm_node_t* node = hm->buckets[hash];

    while (node) {
        if (strncmp(node->key, key, HM_MAX_KEY_LENGTH) == 0) {
            // We need a reference to the next node
            hm_node_t* next = node->next;

            // This node might be the first in the list. If it's not, link the previous node
            // to the next
            if (prevNode) {
                prevNode->next = next;
            } else {
                // Otherwise, this is the first node in the list
                hm->buckets[hash] = next;
            }

            // Free the old node
            free(node);

            // We're done
            return true;
        }
        prevNode = node;
        node = node->next;
    }

    // We didn't find the key we were looking for
    return false;
}

void* hm_get(hashmap_t* hm, const char* key) {
    // Get the hash of the key
    size_t hash = hash_function(key, hm->numBuckets);

    // Attempt to find the node
    hm_node_t* node = hm->buckets[hash];

    while (node) {
        if (strncmp(node->key, key, HM_MAX_KEY_LENGTH) == 0) {
            // This is the item we're looking for
            return node->item;
        }
        node = node->next;
    }

    // We didn't find the key we were looking for
    return NULL;
}

void hm_free(hashmap_t* hm) {
    if (!hm) return;

    // Pointers for iterating through all the nodes as we free them
    hm_node_t* node;
    hm_node_t* next;

    // Work through every bucket, freeing the nodes
    for (size_t i = 0; i < hm->numBuckets; i++) {
        node = hm->buckets[i];

        while (node) {
            next = node->next;
            free(node);
            node = next;
        }
    }

    // After we've freed all the nodes, we can free the hashmap struct itself
    free(hm);
}
