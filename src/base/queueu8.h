/*
queueu8.h - Queue uint8_t Data Structure
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
#ifndef __QueueU8_H__
#define __QueueU8_H__

#pragma once

#include "../common.h"

/* Attributes of the class */
typedef struct QueueU8_Data {
  uint64_t capacity;
  uint8_t* storage;
  uint64_t head;
  uint64_t tail;
  uint64_t usage;
} _QueueU8_Data;

/* QueueU8 Class */
struct QueueU8_Vtbl; /* Forward declaration */
typedef struct __queueu8 {
  struct QueueU8_Vtbl const *vptr; /* Virtual Function Table */
  void const * super; /* Parent Virtual Function Table */
  _QueueU8_Data; /* Attributes of the class (Anonymous struct) */
} QueueU8;

/* QueueU8 Class Virtual Function Table */
typedef struct QueueU8_Vtbl {
  uint8_t (*push)(QueueU8 * const this, uint8_t);
  uint8_t (*pop)(QueueU8 * const this);
  uint8_t (*front)(QueueU8 * const * this);
  uint8_t (*back)(QueueU8 * const * this);
  uint8_t (*isFull)(QueueU8 * const * this);
  uint8_t (*isEmpty)(QueueU8 * const * this);
} _QueueU8_Vtbl;

/* Constructors */
void QueueU8__ctor(QueueU8 * const this, uint64_t capacity);
void QueueU8__dtor(QueueU8 * const this);

#endif /* __QueueU8_H__ */
