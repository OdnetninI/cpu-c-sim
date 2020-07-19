/*
simple.c - Simple Memory class
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
#include "../../common.h"
#include "../mem.h"
#include "simple.h"

#include "../request.h"

void SimpleMem__tick (SimpleMem const * const this) {
  this->super->tick(this);
  printf("SimpleMem ticked\n");
  Request* req = this->fromCPU->vptr->pop(this->fromCPU);
  if (req != nullptr) {
    uint64_t addr = req->vptr->getAddress(req);
    printf("Request for address %x reached memory\n", addr);

    uint8_t* data = malloc(req->vptr->getDataSize(req) * sizeof(uint8_t));
    memcpy(data, this->memory + addr, req->vptr->getDataSize(req));
    req->vptr->setData(req, data);

    this->toCPU->vptr->push(this->toCPU, req);
  }
}

void SimpleMem__initializeMemory(SimpleMem const * const this, uint64_t const address, uint8_t const * const source, uint64_t const size) {
  memcpy(this->memory + address, source, size);
}

void SimpleMem__setCPUQueues(SimpleMem* const this, Queue* const fromCPU, Queue* const toCPU) {
  this->fromCPU = fromCPU;
  this->toCPU = toCPU;
}

static const struct SimpleMem_Vtbl SimpleMem_Vtbl =
{
 .tick = SimpleMem__tick,
 .initializeMemory = SimpleMem__initializeMemory,
 .setCPUQueues = SimpleMem__setCPUQueues,
};

void SimpleMem__ctor(SimpleMem * const this) {
  Mem__ctor(this);
  this->super = this->vptr;
  this->vptr = &SimpleMem_Vtbl;

  this->memory_size = MB(128);
  this->memory = malloc(this->memory_size * sizeof(*(this->memory)));
}

void SimpleMem__dtor(SimpleMem * const this) {
  free(this->memory);
  Mem__dtor(this);
}
