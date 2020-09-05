/*
queueu8.c - Queue uint8_t Data Structure
 */
#include "../common.h"
#include "queueu8.h"

uint8_t QueueU8_push(QueueU8 * const queue, uint8_t object) {
  if (queue->usage >= queue->capacity) return null;
  
  queue->storage[queue->tail] = object;
  queue->tail = (queue->tail + 1) % queue->capacity;
  queue->usage++;
  
  return 1;
}

uint8_t QueueU8_pop(QueueU8 * const queue) {
  if (queue->head == queue->tail) return null;
  /* Queue is not a pointer, be carefull, check that the queue has items */
  
  uint8_t obj = queue->storage[queue->head];
  queue->head = (queue->head + 1) % queue->capacity;
  queue->usage--;
  return obj;
}

uint8_t QueueU8_front(QueueU8 const * const queue) {
  uint8_t obj = queue->storage[queue->head];
  return obj;
}

uint8_t QueueU8_back(QueueU8 const * const queue) {
  uint8_t obj = queue->storage[(queue->head + queue->usage - 1) % queue->capacity];
  return obj;
}

uint8_t QueueU8_isFull(QueueU8 const * const queue) {
  if (queue->usage >= queue->capacity) return 1;
  return 0;
}

uint8_t QueueU8_isEmpty(QueueU8 const * const queue) {
  if (queue->usage == 0) return 1;
  return 0;
}

void QueueU8_init(QueueU8 * const queue, uint64_t capacity) {
  queue->capacity = capacity;
  queue->usage = 0;
  queue->head = 0;
  queue->tail = 0;

  queue->storage = (void*)malloc(capacity * sizeof(*(queue->storage)));
}

void QueueU8_destroy(QueueU8 * const queue) {
  free(queue->storage);
}
