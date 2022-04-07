# Stretchy Buffer aka Vectors

Stretchy buffers behave like arrays, except you don't need to specify their capacity up front, and they automatically expand as you add more items. In this way, they are a lot like the `vector` types you'd find in C++.

I heard about stretchy buffers from [Bitwise](https://github.com/pervognsen/bitwise), Per Vognesen's series about creating a computing platform from the ground up, though [Sean Barret](https://nothings.org/) seems to be the originator of the pattern. This implementation is pretty much identical to what Per shows on Bitwise - so if you want a little more nuance and explanation, check out the video where he codes it up.

The beauty of stretchy buffers is that, to the user, they appear to just be regular arrays accessed using the familar `array[index]` syntax.

```C
float* buf = NULL; // Empty buffer pointing to nothing

// Push some data
sb_pushback(buf, 3.14f);
sb_pushback(buf, 1.0f);
sb_pushback(buf, -10.0f);

for (size_t i = 0; i < sb_length(buf); i++) {
  // Access values just like a regular array
  printf("%d: %f\n", i, buf[i]);
}

// -> 0: 3.140000
// -> 1: 1.000000
// -> 2: -10.000000
```

## The API

`stretchy-buffers.h` defines the basis of all the operations. The public API includes the following "functions":

- `sb_length(bufferPtr)`
- `sb_capacity(bufferPtr)`
- `sb_pushback(bufferPtr, item)`
- `sb_pop(bufferPtr)`
- `sb_free(bufferPtr)`
- `sb_create(itemType, name)`

I put "functions" in quotes there because all of these functions are in fact just macros. There are also a few private "functions", and these all begin with `sb__` - and of course, these shouldn't be used by the end user.

## The core idea

The core idea here is that you really do just have a regular array, only there is a little bit of extra data (the "header") just before the array in memory. That header contains the current capacity (as a `size_t`) and the current length (also a `size_t`). Whenever we want to, say, insert an element using `sb_pushback(bufferPtr, item)`, what happens is we access the header struct (`SBHeader`) just before the array and check if we have enough space for the new item. If we do, great - we simply write the value into the array at index `sb_length(bufferPtr)` and then increment the length. If we don't, we perform a reallocation of the total buffer (including the header), and then insert the element in the same way.

The `SBHeader` struct looks like this:

```C
typedef struct {
  // The actual header information
  size_t length;
  size_t capacity;
  // A named reference to the start of the data (takes no actual memory)
  uint8_t data[0];
} SBHeader;
```

Depending on your viewpoint, you might see that 0-length data array as a pretty clever way to give a name to the space where the users data *will* be, or as an absolute abomination that has no place in a codebase. I'm not making any value judgements here, but I will say that I've seen worse things in my life.

So where does this header actually come from? As previously mentioned, when we use an operation like `sb_pushback`, we're checking if we have the capacity for the new element. The `sb_capacity` macro looks like this:

```C
#define sb_capacity(b) ((b) ? sb__header(b)->capacity : 0)
```

There is an implicit `NULL` check happening here in the ternary, which will evaluate to `0` if the buffer hasn't yet been initialised. `sb_length` is defined in exactly the same way. The result is that when the current capacity is checked, it will evaluate to false in both the case where the buffer is initialised but there isn't space, and the case where the buffer wasn't initialised to begin with. When not enough space is available, a call is made to the only real function in the API: `void* sb__grow(void* data, size_t itemSize)`

```C
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
```

This function does the heavy lifting. First it calculates what the new capacity will be. You can use various growth strategies here, but this one doubles the size of the buffer whenever space runs out. Next it calculates the size of the new buffer - and these are two different things. For one thing, this size includes the header, but it also needs to account for the size of each item. If the buffer is still NULL, it gets `malloc`d for the first time, otherwise it is `realloc`d. And of course, the *actual* pointer that was obtained through `malloc` was the header itself, so that's what we need to pass. But the user shouldn't be exposed to that, so we return a pointer to the data itself.

You can take a closer look at [stretchy-buffer.h](./stretchy-buffer.h) to see how all of the operations are defined.