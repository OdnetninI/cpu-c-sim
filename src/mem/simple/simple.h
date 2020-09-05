/*
simple.h - Simple Memory class
 */
#ifndef __SIMPLEMEM_H__
#define __SIMPLEMEM_H__

#pragma once

#include "../../common.h"
#include "../../base/queue.h"

/* Attributes of the class */
typedef struct _mem {  
  Queue* fromCPU;
  Queue* toCPU;

  uint64_t memory_size; /* Size of the current memory */
  uint8_t* memory; /* Allocable memory */
} Mem;

/* Constructors */
void Mem_init(Mem * const mem);
void Mem_destroy(Mem * const mem);

void Mem_tick (Mem const * const mem);
void Mem_initializeMemory(Mem const * const mem, uint64_t const address, uint8_t const * const source, uint64_t const size);
void Mem_setCPUQueues(Mem * const mem, Queue* const fromCPU, Queue* const toCPU);

#endif /* __SIMPLEMEM_H__ */
