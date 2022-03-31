#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "fifo.h"

void print_fifo_info(fifo_t* fifo) {
  printf("Length: %d\nEmpty: %d\nFull: %d\n", fifo->size, fifo_empty(fifo), fifo_full(fifo));
  printf("Head: %d\nTail: %d\n", fifo->head, fifo->tail);
  printf("-----------------\n");
}

void print_event_info(fifo_event_t* event) {
  if (event != NULL) {
    printf("Event Type: %d\nData Address: 0x%p\n", event->event_type, event->data);
  } else {
    printf("[!] Pulled from an empty fifo!\n");
  }
}

void test_basics() {
  fifo_t fifo;
  fifo_t* p_fifo = &fifo;
  fifo_event_t* out;

  printf("> Init\n");
  fifo_init(p_fifo);
  print_fifo_info(p_fifo);

  printf("> Pull from empty\n");
  out = fifo_pull(p_fifo);
  print_event_info(out);
  print_fifo_info(p_fifo);

  printf("> Peek from empty\n");
  out = fifo_peek(p_fifo);
  print_event_info(out);
  print_fifo_info(p_fifo);

  printf("> Push event 1\n");
  fifo_push(p_fifo, 1, NULL);
  print_fifo_info(p_fifo);

  printf("> Peek at event 1\n");
  out = fifo_peek(p_fifo);
  print_event_info(out);
  print_fifo_info(p_fifo);

  printf("> Push event 2\n");
  fifo_push(p_fifo, 2, p_fifo);
  print_fifo_info(p_fifo);

  printf("> Push event 3\n");
  fifo_push(p_fifo, 3, &out);
  print_fifo_info(p_fifo);

  printf("> Pull event 1\n");
  out = fifo_pull(p_fifo);
  print_event_info(out);
  print_fifo_info(p_fifo);

  printf("> Push event 4\n");
  fifo_push(p_fifo, 4, NULL);
  print_fifo_info(p_fifo);

  printf("> Pull event 2\n");
  out = fifo_pull(p_fifo);
  print_event_info(out);
  print_fifo_info(p_fifo);

  printf("> Pull event 3\n");
  out = fifo_pull(p_fifo);
  print_event_info(out);
  print_fifo_info(p_fifo);

  printf("> Pull event 4\n");
  out = fifo_pull(p_fifo);
  print_event_info(out);
  print_fifo_info(p_fifo);

  printf("> Pull from empty\n");
  out = fifo_pull(p_fifo);
  print_event_info(out);
  print_fifo_info(p_fifo);
}

fifo_t a;
fifo_t b;

void* thread_a(void* arg) {
  printf("[a] Starting\n");
  printf("[a] Sleeping 2 seconds...\n");
  sleep(2);
  printf("[a] Pushing event to b...\n");
  fifo_push(&b, 1, NULL);

  printf("[a] Waiting for response\n");
  while (fifo_empty(&a)) {}
  fifo_event_t* out = fifo_pull(&a);
  printf("[a] Got event %d\n", out->event_type);

  pthread_exit(NULL);
}

void* thread_b(void* arg) {
  printf("[b] Starting\n");
  printf("[b] Waiting for response\n");
  while (fifo_empty(&b)) {}
  fifo_event_t* out = fifo_pull(&b);
  printf("[b] Got event %d\n", out->event_type);

  printf("[b] Sleeping 2 seconds...\n");
  sleep(2);
  printf("[b] Pushing event to a...\n");
  fifo_push(&a, 2, NULL);

  pthread_exit(NULL);
}

void test_threads() {
  fifo_init(&a);
  fifo_init(&b);

  pthread_t ta;
  pthread_t tb;

  pthread_create(&ta, NULL, thread_a, NULL);
  pthread_create(&tb, NULL, thread_b, NULL);

  pthread_join(ta, NULL);
  pthread_join(tb, NULL);
}

int main() {
  test_basics();

  return 0;
}
