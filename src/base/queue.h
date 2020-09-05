/*
queue.h - Queue Data Structure
 */
#ifndef __Queue_H__
#define __Queue_H__

#pragma once

#include "../common.h"

/* Attributes of the class */
typedef struct _queue {
  uint64_t capacity;
  void** storage;
  uint64_t head;
  uint64_t tail;
  uint64_t usage;
} Queue;

/* Constructors */
void Queue_init(Queue * const queue, uint64_t capacity);
void Queue_destroy(Queue * const queue);

uint8_t Queue_push(Queue * const queue, void* object);
void* Queue_pop(Queue * const queue);
void* Queue_front(Queue const * const queue);
void* Queue_back(Queue const * const queue);
uint8_t Queue_isFull(Queue const * const queue);
uint8_t Queue_isEmpty(Queue const * const queue);

#endif /* __Queue_H__ */
