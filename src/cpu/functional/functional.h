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

typedef struct _cpu {  
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
  
} CPU;

void CPU_init(CPU * const cpu);
void CPU_destroy(CPU * const cpu);

void CPU_tick (CPU * const cpu);
void CPU_setMemoryQueues(CPU * const cpu, Queue* const toMemory, Queue* const fromMemory);
void CPU_setPC(CPU * const cpu, uint64_t const pc);

#endif /* __CPU_H__ */
