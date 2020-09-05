/*
simple.c - Simple Memory class
 */
#include "../../common.h"
#include "simple.h"
#include "../request.h"

void Mem_tick (Mem const * const mem) {
  Request* req = Queue_pop(mem->fromCPU);
  if (req != nullptr) {
    uint64_t addr = Request_getAddress(req);
    /*printf("Request for address %x reached memory\n", addr);*/

    uint8_t* data = malloc(Request_getDataSize(req) * sizeof(uint8_t));
    memcpy(data, mem->memory + addr, Request_getDataSize(req));
    Request_setData(req, data);

    Queue_push(mem->toCPU, req);
  }
}

void Mem_initializeMemory(Mem const * const mem, uint64_t const address, uint8_t const * const source, uint64_t const size) {

  uint64_t i = 0;
  uint8_t* dest = mem->memory + address;
  
  for(i = 0; i < size; ++i)
    dest[i] = source[i];

  /* In future make aligned part with mem copy, and unaligned part with for loop */
  /* memcpy(mem->memory + address, source, size); */
}

void Mem_setCPUQueues(Mem * const mem, Queue* const fromCPU, Queue* const toCPU) {
  mem->fromCPU = fromCPU;
  mem->toCPU = toCPU;
}

void Mem_init(Mem * const mem) {
  mem->memory_size = MB(128);
  mem->memory = malloc(mem->memory_size * sizeof(*(mem->memory)));
}

void Mem_destroy(Mem * const mem) {
  free(mem->memory);
}
