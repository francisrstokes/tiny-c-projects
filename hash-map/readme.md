# Hashmap Data Structure From Scratch

*Note: This implementation draws a lot from the far more in-depth [blog post put together by Andrei Ciobanu](https://www.andreinc.net/2021/10/02/implementing-hash-tables-in-c-part-1). Be sure to check that out if you want to see how these ideas are taken the next level!*

Hashmaps, sometimes known as *hash tables*, *associative arrays*, or *dictionaries*, allow mapping a given value to a **key** that represents that value. These are fundamental data structures in almost every language, and are invaluable when solving certain kinds of problems.

In this project, in order to keep it simple and focussed, the hashmap implementation will have string keys, and arbitrary types as values. Data will be stored in the map by reference, meaning it's the user's responsibility to allocate the memory for any given item. With a little tweaking, you can easily write an implementation where keys can be any kind of data, and primitive values can be stored directly.

## Mechanism

The way they actually operate is a lot like a regular array. In an array, you can store a fixed number of elements of the same type, using a numerical index to select the individual items. The basis of the hashmap in this project is also an array. The string key is transformed into an index by running it through a hashing function (hence the name hashmap).

Hashing functions are an entire can of worms in and of themselves - ranging from simple techniques like adding all of the characters modulo the size of the array, to much more complex techniques where mathematical/cryptographic properties are taken into account.

The biggest challenge when selecting a hashing function is that different keys can end up producing the same hash, which is known as a collision. Collisions are par for the course in a hashmap, but some functions will do a much better job at reducing the number of collisions that occur. In a sense, what you're looking for is for in a hashing function is generating a seemingly random index, where the randomness has a uniform distribution (every index is as likely as any other).

Without going into too much theory, this is the implementation of the hash function (found in src/hash.c):

```C
#define DJB2_INIT 5381
size_t hash_function(const char* ptr, const size_t buckets) {
    size_t hash = DJB2_INIT;

    uint8_t byte;
    while ((byte = *ptr++)) {
        hash = (size_t)(((hash << 5) + hash) + byte);
    }

    return hash % buckets;
}
```

This function is called DJB2, and it works by starting out with an initial seed value (`DJB2_INIT`), and and iteratively modifying this value with each byte of the key, mixing up the hash each time. Let's focus on the line that's doing the hard work and remove some of the noise:

```C
((hash << 5) + hash) + byte
```

Each new `hash` value is the result of this line. First, the existing value is mixed in with itself by left shifting the current value and then adding the value. Another way of thinking about this operation just:

```C
hash * 33 + byte
```

Left shifting is really just multiplication by a power of 2 (`2^5 == 32, + 1 = 33`). Why 33 you ask? Well, it seems to work the best! Apparently no formal explanation has ever really be given as to why. Same thing for the initialisation value `5381` - it just works well. I know, it was unsatisfying for me too.

In the end, whatever the result of all of this was is reduced to a fixed range with the modulo operator.

## The rest of the owl

The hash function is the key here (no pun intended), but it's not all there is. We need a structure to represent the hash map itself, which looks like this (found in inc/hash.h):

```C
typedef struct {
    // Configurable number of buckets
    size_t numBuckets;
    // Pointer to a yet-to-be-created array of buckets
    hm_node_t** buckets;
} hashmap_t;
```

It holds two properties: a number of buckets, and the buckets themselves. A bucket is a space that represents a given hash. When we create a hashmap, we might give ourselves 100 buckets. This means there can only be 100 individual hashes. Colliding hashes map into the same bucket.

The user actually creates a `hashmap_t` by calling `hm_create`, which looks like this (found in src/hash.c):

```C
hashmap_t* hm_create(const size_t buckets) {
    hashmap_t* hm = malloc(sizeof(hashmap_t));
    hm->numBuckets = buckets;
    hm->buckets = malloc(sizeof(hm_node_t*) * buckets);

    return hm;
}
```

Two allocations take place. The first is reserving enough space to store the `hashmap_t` structure itself. The second is space for a contiguous array of `hm_node_t*` buckets. A bucket has the type `hm_node_t`, which is defined like so:

```C
typedef struct hm_node_t {
    // Regular linked list stuff
    struct hm_node_t* next;

    // Store a pointer to the item data
    void* item;

    // Store the key so we can properly retrieve items
    char key[0];
} hm_node_t;
```

It's a linked list! Well, in this implementation it is anyway - there are other ways of doing this. As well as storing a pointer to the next node in the linked list, it also stores a (void) pointer to the item (the value associated with a key), and the key itself. A tricky note here is that the key is stored as a 0-element array of characters - meaning it apparently takes up no space in this struct. This is done because we don't know the size of the key until the user places data into the hashmap. When they do, we can allocate enough memory for this struct, plus as much as we need for the key. Since the key is the last member of the struct, it won't clobber any other items.

So why a linked list? Because keys can collide with each other. When we go to retrieve an item by key, we have to hash the key to get the bucket index. Then we get the first node in the bucket and compare the stored key with the key the user gave us. If the key doesn't match, we go to the next node in the list and check that, until we're out of nodes. The implementation of `hm_get` looks something like this (found in src/hash.c):

```C
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
```

The remaining operations, `hm_set` and `hm_remove` work in a very similar way; First get the hash, and then traverse the nodes until the one matching the key is found. Then the data can be updated in the case of `hm_set`, or the node spliced out in the case of `hm_remove`. In both cases, of course, the the lookup may fail if the key wasn't in the hashmap in the first place. A little extra code makes sure these are properly handled too. You can see the full details in src/hash.c - it's about 100 lines with the comments and extra spacing stripped out.

## Improvements

There are a bunch of improvements and optimistations to be made, and you can find more information about that over at [Andrei's blog](https://www.andreinc.net/2021/10/02/implementing-hash-tables-in-c-part-1).

An immediate and simple improvement would be use [stretchy buffers AKA vectors](../stretchy-buffers/readme.md) instead of linked lists to store collections of items in the buckets. This can have a big impact on cache locality when often used keys/buckets are accessed - especially when the number of buckets is small.

