/*
queue.h - Queue Data Structure
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
#ifndef __Queue_H__
#define __Queue_H__

#pragma once

#include "../common.h"

/* Attributes of the class */
typedef struct Queue_Data {
  uint64_t capacity;
  void** storage;
  uint64_t head;
  uint64_t tail;
  uint64_t usage;
} _Queue_Data;

/* Queue Class */
struct Queue_Vtbl; /* Forward declaration */
typedef struct __queue {
  struct Queue_Vtbl const *vptr; /* Virtual Function Table */
  void const * super; /* Parent Virtual Function Table */
  _Queue_Data; /* Attributes of the class (Anonymous struct) */
} Queue;

/* Queue Class Virtual Function Table */
typedef struct Queue_Vtbl {
  uint8_t (*push)(Queue * const this, void* object);
  void* (*pop)(Queue * const this);
  void* (*front)(Queue * const * this);
  void* (*back)(Queue * const * this);
  uint8_t (*isFull)(Queue * const * this);
  uint8_t (*isEmpty)(Queue * const * this);
} _Queue_Vtbl;

/* Constructors */
void Queue__ctor(Queue * const this, uint64_t capacity);
void Queue__dtor(Queue * const this);

#endif /* __Queue_H__ */
