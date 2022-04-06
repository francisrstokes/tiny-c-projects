# FIFO For Thread Communication and Synchronisation

A minimal implementation of a circular fifo buffer that can be used for communication and synchronisation between threads, or just a buffer between data producers and consumers.

Put simply, a fifo is a buffer with two key operations - sometimes called `push` and `pull` (but also `put` and `get`, as well as many other similar names). It stands for "First in, first out", which indicates that the order that you put items into the queue is the order that you take them out again. This is in contrast to a stack which has a lifo arrangement - "Last in, first out".

Fifos can be found in both software and hardware, concrete and ad-hoc. One of their super powers is syncing up data producers and consumers, especially when they produce and consume at different rates.

They typically come with two further operations for determining whether the queue is empty or full. Sometimes (especially in hardware), they also have an "almost full" and "almost empty".

## fifo.h

`fifo.h` contains the interface, containing a typedef for `fifo_t`, which holds the state and data of the queue, as well as a typedef for `fifo_event_t` - which is the type of the items that are pushed into the queue.

The event type uses a `uint32_t` type to *tag* item, as well as a void pointer to data - allowing the user to attach whatever data to events they like.

The `fifo_t` type contains a mutex which has to be acquired by all of the operations for thread safety. The queue size can be configured with a `#define FIFO_QUEUE_SIZE` before including `fifo.h`, or via compiler flags. The queue itself is a circular buffer, which means that pushing or pulling from the queue does not necessitate shifting all of the elements.

6 functions are defined for interacting with a fifo:

- `fifo_init` - for initialising the `fifo_t` data structure
- `fifo_full` - for checking if the queue is full
- `fifo_empty` - for checking if the queue is empty
- `fifo_pull` - for pulling an item from the queue
- `fifo_peek` - for accessing the next item in the queue without consuming it
- `fifo_push` - for placing an item into the queue

## Patterns

This library can be used to pass data between two threads, and force synchronisation points. `main.c` contains a small example of two threads and two corresponding fifos - one where thread `A` pushes messages to thread `B`, and one where thread `B` pushes messages to thread `A`.

If thread `A` is producing data slower than thread `B` wants to consume it, `B` can do something like:

```C
#define WAIT_FOR_DATA(fifoPtr) (while (fifo_empty(fifoPtr)) {})

// ...
// In the thread b code

fifo_event_t* ev;

while (1) {
  // Wait until A has data
  WAIT_FOR_DATA(&fifo_a_to_b);

  // Get the next event
  ev = fifo_pull(&fifo_a_to_b);

  // Do something with ev
}
```
