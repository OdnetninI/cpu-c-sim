/*
functional.h - Functional CPU Model
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
#ifndef __FUNCTIONAL_H__
#define __FUNCTIONAL_H__

#pragma once

#include "../../common.h"
#include "../../base/queue.h"
#include "../cpu.h"

/* FunctionalCPU Class Attributes */
struct FunctionalCPU_Vtbl; /* Forward declaration */
typedef struct __func_cpu {
  struct FunctionalCPU_Vtbl const *vptr; /* Virtual Function Table */
  struct CPU_Vtbl const *super; /* Parent Virtual Function Table */

  Queue* toMemory;
  Queue* fromMemory;
  
  uint64_t pc; /* Program Counter */
} FunctionalCPU;

/* FunctionalCPU Class Virtual Function Table */
typedef struct FunctionalCPU_Vtbl {
  _CPU_Vtbl;
  void (*setMemoryQueues)(FunctionalCPU* const this, Queue* const toMemory, Queue* const fromMemory);
} _FunctionalCPU_Vtbl;

/* Constructors */
void FunctionalCPU__ctor(FunctionalCPU * const this);
void FunctionalCPU__dtor(FunctionalCPU * const this);

#endif /* __CPU_H__ */
