#ifndef INC_STRETCHY_BUFFER_H
#define INC_STRETCHY_BUFFER_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
  // The actual header information
  size_t length;
  size_t capacity;
  // A named reference to the start of the data (takes no actual memory)
  uint8_t data[0];
} SBHeader;

// The only real function in the API, used to initialise and grow the buffer
void* sb__grow(void* data, size_t itemSize);

// Convert a pointer to a regular buffer to a pointer to a header
#define sb__header(b) ((SBHeader*)((uint8_t*)b - offsetof(SBHeader, data)))

// Check if n items can fit inside the buffer without a resize
#define sb__fits(b, n) (sb_length(b) + n <= sb_capacity(b))

// Determine if the buffer needs to be resized, then resize if needed
#define sb__fit(b, n) (sb__fits(b, n) ? 0 : ((b) = (sb__grow((b), sizeof(*(b))))))

// Get the current length
#define sb_length(b) ((b) ? sb__header(b)->length : 0)

// Get the current capacity
#define sb_capacity(b) ((b) ? sb__header(b)->capacity : 0)

// Push an item
#define sb_pushback(b, item) (sb__fit(b, 1), b[sb_length(b)] = (item), sb__header(b)->length++)

// Pop an item
#define sb_pop(b) (sb_length(b) > 0 ? (sb__header(b)->length--, &b[sb_length(b)]) : 0)

// Free the stretchy buffer
#define sb_free(b) ((b) ? free(sb__header(b)) : 0)

// Convenience function for creating a stretchy buffer, where you can't forget to assign NULL
#define sb_create(type, name) type* name = NULL;

#endif
