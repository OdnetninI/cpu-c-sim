/*
queue.c - Queue Data Structure
 */
#include "../common.h"
#include "queue.h"

uint8_t Queue_push(Queue * const queue, void* object) {
  if (queue->usage >= queue->capacity) return 0;
  
  queue->storage[queue->tail] = object;
  queue->tail = (queue->tail + 1) % queue->capacity;
  queue->usage++;
  
  return 1;
}

void* Queue_pop(Queue * const queue) {
  if (queue->head == queue->tail) return nullptr;
  
  void* obj = queue->storage[queue->head];
  queue->head = (queue->head + 1) % queue->capacity;
  queue->usage--;
  return obj;
}

void* Queue_front(Queue const * const queue) {
  void* obj = queue->storage[queue->head];
  return obj;
}

void* Queue_back(Queue const * const queue) {
  void* obj = queue->storage[(queue->head + queue->usage - 1) % queue->capacity];
  return obj;
}

uint8_t Queue_isFull(Queue const * const queue) {
  if (queue->usage >= queue->capacity) return 1;
  return 0;
}

uint8_t Queue_isEmpty(Queue const * const queue) {
  if (queue->usage == 0) return 1;
  return 0;
}

void Queue_init(Queue * const queue, uint64_t capacity) { 
  queue->capacity = capacity;
  queue->usage = 0;
  queue->head = 0;
  queue->tail = 0;

  queue->storage = (void*)malloc(capacity * sizeof(*(queue->storage)));
}

void Queue_destroy(Queue * const queue) {
  free(queue->storage);
}
