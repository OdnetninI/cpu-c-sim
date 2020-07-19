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

static const struct Queue_Vtbl Queue_Vtbl =
{
 .push = Queue__push,
 .pop = Queue__pop
};

void Queue__ctor(Queue * const this, uint64_t capacity) {
  this->vptr = &Queue_Vtbl;
  
  this->capacity = capacity;
  this->usage = 0;
  this->head = 0;
  this->tail = 0;

  this->storage = (void*)malloc(capacity * sizeof(*(this->storage)));
}

void Queue__dtor(Queue * const this) {
  free(this->storage);
}
