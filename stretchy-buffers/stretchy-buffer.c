#include "stretchy-buffer.h"

// Every good C program needs to redefine MAX for the billionth time
#define MAX(a, b) ((a) < (b) ? (b) : (a))

void* sb__grow(void* data, size_t itemSize) {
  SBHeader* header;

  // Double the previous capacity (or set it to 1 if this is init)
  size_t newCapacity = MAX(1, sb_capacity(data) * 2);
  size_t newSize = offsetof(SBHeader, data) + newCapacity * itemSize;

  // The buffer isn't yet initialised
  if (data == NULL) {
    header = malloc(newSize);
    header->capacity = newCapacity;
    header->length = 0;
  } else {
    // The buffer has been previously allocated, reallocate more space
    header = realloc(sb__header(data), newSize);
    header->capacity = newCapacity;
  }

  // Give the user back their data
  return &header->data;
}
