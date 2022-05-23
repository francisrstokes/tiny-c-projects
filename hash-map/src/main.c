#include <stdio.h>
#include "hash.h"

void printGetResult(hashmap_t* hm, const char* key);

// Hash map will only have 10 buckets
#define BUCKET_SIZE 10

// Define some keys to use for testing
#define KEY1 "Some key"
#define KEY2 "Another key"
#define KEY3 "Key number 3....." // This key has the same hash as KEY0 with 10 buckets
#define KEY4 "Key number 4"

int main() {
    hashmap_t* hm = hm_create(BUCKET_SIZE);

    uint32_t value1 = 0xdeadbeef;
    uint32_t value2 = 0xc0decafe;
    uint32_t value3 = 0xf00dbabe;
    uint32_t value4 = 0xfeedc0de;

    hm_set(hm, KEY1, &value1);
    hm_set(hm, KEY2, &value2);
    hm_set(hm, KEY3, &value3);
    hm_set(hm, KEY4, &value4);

    printGetResult(hm, KEY1);
    printGetResult(hm, KEY2);
    printGetResult(hm, KEY3);
    printGetResult(hm, KEY4);
    printGetResult(hm, "Not here!");

    printf("  [i] Removing key '%s'\n", KEY1);
    hm_remove(hm, KEY1);
    printGetResult(hm, KEY1);
    printGetResult(hm, KEY3);

    printf("  [i] Setting new value for key '%s'\n", KEY1);
    value1 = 0x123;
    hm_set(hm, KEY1, &value1);
    printGetResult(hm, KEY1);
    printGetResult(hm, KEY3);

    hm_free(hm);

    return 0;
}

void printGetResult(hashmap_t* hm, const char* key) {
    uint32_t* valuePtr;
    valuePtr = hm_get(hm, key);
    if (valuePtr) {
        printf("[%s]: %x\n", key, *valuePtr);
    } else {
        printf("[%s]: Key not found!\n", key);
    }
}
