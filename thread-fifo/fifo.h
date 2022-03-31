#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <semaphore.h>

#ifndef FIFO_QUEUE_SIZE
#define FIFO_QUEUE_SIZE 128
#endif

typedef struct {
  uint32_t event_type;
  void* data;
} fifo_event_t;

typedef struct {
  sem_t mutex;
  fifo_event_t queue[FIFO_QUEUE_SIZE];
  size_t head;
  size_t tail;
  size_t size;
} fifo_t;

void fifo_init(fifo_t* fifo);
bool fifo_full(fifo_t* fifo);
bool fifo_empty(fifo_t* fifo);
fifo_event_t* fifo_pull(fifo_t* fifo);
fifo_event_t* fifo_peek(fifo_t* fifo);
bool fifo_push(fifo_t* fifo, uint32_t event_type, void* data);

#endif // MAIN_H
