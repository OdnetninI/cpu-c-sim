/*
simple.h - Simple Memory class
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
#ifndef __SIMPLEMEM_H__
#define __SIMPLEMEM_H__

#pragma once

#include "../../common.h"
#include "../../base/queue.h"
#include "../mem.h"

/* Attributes of the class */
typedef struct SimpleMem_Data {
  _Mem_Data; /* Inherited attributes from parents */
  
  Queue* fromCPU;
  Queue* toCPU;

  uint64_t memory_size; /* Size of the current memory */
  uint8_t* memory; /* Allocable memory */
} _SimpleMem_Data;

/* Mem Class */
struct SimpleMem_Vtbl; /* Forward declaration */
typedef struct __simplemem {
  struct SimpleMem_Vtbl const *vptr; /* Virtual Function Table */
  struct Mem_Vtbl const *super; /* Parent Virtual Function Table */
  _SimpleMem_Data; /* Attributes of the class (Anonymous struct) */
} SimpleMem;

/* SimpleMem Class Virtual Function Table */
typedef struct SimpleMem_Vtbl {
  _Mem_Vtbl;
  void (*setCPUQueues)(SimpleMem* const this, Queue* const fromCPU, Queue* const toCPU);
} _SimpleMem_Vtbl;

/* Constructors */
void SimpleMem__ctor(SimpleMem * const this);
void SimpleMem__dtor(SimpleMem * const this);

#endif /* __SIMPLEMEM_H__ */
