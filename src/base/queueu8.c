/*
queueu8.c - Queue uint8_t Data Structure
Copyright (C) 2020 OdnetninI

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; If not, see <http://www.gnu.org/licenses/>.
 */
#include "../common.h"
#include "queueu8.h"

uint8_t QueueU8__push(QueueU8 * const this, uint8_t object) {
  if (this->usage >= this->capacity) return null;
  
  this->storage[this->tail] = object;
  this->tail = (this->tail + 1) % this->capacity;
  this->usage++;
  
  return 1;
}

uint8_t QueueU8__pop(QueueU8 * const this) {
  if (this->head == this->tail) return null;
  /* This is not a pointer, be carefull, check that the queue has items */
  
  void* obj = this->storage[this->head];
  this->head = (this->head + 1) % this->capacity;
  this->usage--;
  return obj;
}

uint8_t QueueU8__front(QueueU8 const * const this) {
  void* obj = this->storage[this->head];
  return obj;
}

uint8_t QueueU8__back(QueueU8 const * const this) {
  void* obj = this->storage[(this->head + this->usage - 1) % this->capacity];
  return obj;
}

uint8_t QueueU8__isFull(QueueU8 const * const this) {
  if (this->usage >= this->capacity) return 1;
  return 0;
}

uint8_t QueueU8__isEmpty(QueueU8 const * const this) {
  if (this->usage == 0) return 1;
  return 0;
}

static const struct QueueU8_Vtbl QueueU8_Vtbl =
{
 .push = QueueU8__push,
 .pop = QueueU8__pop,
 .front = QueueU8__front,
 .back = QueueU8__back,
 .isFull = QueueU8__isFull,
 .isEmpty = QueueU8__isEmpty,
};

void QueueU8__ctor(QueueU8 * const this, uint64_t capacity) {
  this->vptr = &QueueU8_Vtbl;
  this->super = nullptr;
  
  this->capacity = capacity;
  this->usage = 0;
  this->head = 0;
  this->tail = 0;

  this->storage = (void*)malloc(capacity * sizeof(*(this->storage)));
}

void QueueU8__dtor(QueueU8 * const this) {
  free(this->storage);
}
