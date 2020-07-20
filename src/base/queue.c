/*
queue.c - Queue Data Structure
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
#include "queue.h"

uint8_t Queue__push(Queue * const this, void* object) {
  if (this->usage >= this->capacity) return 0;
  
  this->storage[this->tail] = object;
  this->tail = (this->tail + 1) % this->capacity;
  this->usage++;
  
  return 1;
}

void* Queue__pop(Queue * const this) {
  if (this->head == this->tail) return nullptr;
  
  void* obj = this->storage[this->head];
  this->head = (this->head + 1) % this->capacity;
  this->usage--;
  return obj;
}

void* Queue__front(Queue const * const this) {
  void* obj = this->storage[this->head];
  return obj;
}

void* Queue__back(Queue const * const this) {
  void* obj = this->storage[(this->head + this->usage - 1) % this->capacity];
  return obj;
}

uint8_t Queue__isFull(Queue const * const this) {
  if (this->usage >= this->capacity) return 1;
  return 0;
}

uint8_t Queue__isEmpty(Queue const * const this) {
  if (this->usage == 0) return 1;
  return 0;
}

static const struct Queue_Vtbl Queue_Vtbl =
{
 .push = Queue__push,
 .pop = Queue__pop,
 .front = Queue__front,
 .back = Queue__back,
 .isFull = Queue__isFull,
 .isEmpty = Queue__isEmpty,
};

void Queue__ctor(Queue * const this, uint64_t capacity) {
  this->vptr = &Queue_Vtbl;
  this->super = nullptr;
  
  this->capacity = capacity;
  this->usage = 0;
  this->head = 0;
  this->tail = 0;

  this->storage = (void*)malloc(capacity * sizeof(*(this->storage)));
}

void Queue__dtor(Queue * const this) {
  free(this->storage);
}
