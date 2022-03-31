#include "fifo.h"

void fifo_init(fifo_t* fifo) {
  fifo->head = 0;
  fifo->tail = 0;
  fifo->size = 0;
  sem_init(&fifo->mutex, 1, 1);
}

bool fifo_full(fifo_t* fifo) {
  sem_wait(&fifo->mutex);
  bool result = fifo->size >= FIFO_QUEUE_SIZE;
  sem_post(&fifo->mutex);
  return result;
}

bool fifo_empty(fifo_t* fifo) {
  sem_wait(&fifo->mutex);
  bool result = fifo->size == 0;
  sem_post(&fifo->mutex);
  return result;
}

fifo_event_t* fifo_pull(fifo_t* fifo) {
  sem_wait(&fifo->mutex);

  // If there's nothing to pull, return nullptr
  if (fifo->size == 0) {
    sem_post(&fifo->mutex);
    return NULL;
  }

  size_t head = fifo->head;
  fifo_event_t* event = &fifo->queue[head];

  if (head == FIFO_QUEUE_SIZE - 1) {
    fifo->head = 0;
  } else {
    ++(fifo->head);
  }

  --(fifo->size);

  sem_post(&fifo->mutex);

  return event;
}

fifo_event_t* fifo_peek(fifo_t* fifo) {
  sem_wait(&fifo->mutex);

  // If there's nothing to pull, return nullptr
  if (fifo->size == 0) {
    sem_post(&fifo->mutex);
    return NULL;
  }

  fifo_event_t* event = &fifo->queue[fifo->head];

  sem_post(&fifo->mutex);

  return event;
}

bool fifo_push(fifo_t* fifo, uint32_t event_type, void* data) {
  sem_wait(&fifo->mutex);

  if (fifo->size >= FIFO_QUEUE_SIZE) {
    sem_post(&fifo->mutex);
    return false;
  }

  size_t tail = fifo->tail;
  fifo->queue[tail].data = data;
  fifo->queue[tail].event_type = event_type;

  if (tail == FIFO_QUEUE_SIZE - 1) {
    fifo->tail = 0;
  } else {
    ++(fifo->tail);
  }

  ++(fifo->size);

  sem_post(&fifo->mutex);

  return true;
}
