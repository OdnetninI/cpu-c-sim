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
#include "../../base/queueu8.h"
#include "../cpu.h"

typedef struct _instruction {
  struct {
    uint8_t opsize : 1;
    uint8_t address : 1;
    uint8_t lock : 1;
    uint8_t rep : 1;
    uint8_t repme : 1;
    uint8_t vex2 : 1;
    uint8_t vex3 : 1;
    uint8_t res : 1;
    uint8_t seg;
    uint8_t rex;
  } prefix;
  uint8_t type;
  uint8_t opcode;
} X86Inst;

/* Attributes of the class */
typedef struct FunctionalCPU_Data {
  _CPU_Data; /* Inherited attributes from parents */
  
  Queue* toMemory;
  Queue* fromMemory;

  uint64_t pc; /* Program Counter */

  /* Fetch Data */
  uint64_t fetchState;
  uint64_t fetchBytes;
  uint8_t currentFetchByte;

  /* Decode Data */
  uint64_t decodeState;
  QueueU8* decodeBytes;
  X86Inst decodedInst;
  
} _FunctionalCPU_Data;

/* FunctionalCPU Class */
struct FunctionalCPU_Vtbl; /* Forward declaration */
typedef struct __func_cpu {
  struct FunctionalCPU_Vtbl const *vptr; /* Virtual Function Table */
  struct CPU_Vtbl const *super; /* Parent Virtual Function Table */
  _FunctionalCPU_Data; /* Attributes of the class (Anonymous struct) */
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
