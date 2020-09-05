/*
queueu8.h - Queue uint8_t Data Structure
 */
#ifndef __QueueU8_H__
#define __QueueU8_H__

#pragma once

#include "../common.h"

/* Attributes of the class */
typedef struct _queueU8 {
  uint64_t capacity;
  uint8_t* storage;
  uint64_t head;
  uint64_t tail;
  uint64_t usage;
} QueueU8;

/* Constructors */
void QueueU8_init(QueueU8 * const queue, uint64_t capacity);
void QueueU8_destroy(QueueU8 * const queue);

uint8_t QueueU8_push(QueueU8 * const queue, uint8_t object);
uint8_t QueueU8_pop(QueueU8 * const queue);
uint8_t QueueU8_front(QueueU8 const * const queue);
uint8_t QueueU8_back(QueueU8 const * const queue);
uint8_t QueueU8_isFull(QueueU8 const * const queue);
uint8_t QueueU8_isEmpty(QueueU8 const * const queue);

#endif /* __QueueU8_H__ */
