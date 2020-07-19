/*
functional.c - Functional CPU Model
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
#include "../cpu.h"
#include "functional.h"
#include "../../mem/request.h"

enum FetchState {
		 RequestToMemory = 0,
		 WaitingToMemory,
		 SendToDecode
};

void FunctionalCPU__fetch(FunctionalCPU * const this) {
  switch(this->fetchState) {
  case RequestToMemory:
    {
      Request* req = new(Request);
      req->vptr->setDataSize(req, 4);
      req->vptr->setAddress(req, this->pc);
      this->toMemory->vptr->push(this->toMemory, req);
      printf("Sent pc %x request to mem\n", this->pc);
    
      this->fetchState = WaitingToMemory;
      break;
    }
  case WaitingToMemory:
    {
      Request* req = this->fromMemory->vptr->pop(this->fromMemory);
      if (req != nullptr) {
	uint64_t addr = req->vptr->getAddress(req);
	printf("Address %x answered from memory\n", addr);

	int i = 0;
	for (i = 0; i < req->vptr->getDataSize(req); ++i)
	  printf("0x%02x ", req->vptr->getData(req)[i]);
	printf("\n");

	free(req->vptr->getData(req));
	Request__dtor(req);
	delete(req);

	this->fetchState = SendToDecode;
      }
      break;
    }
  case SendToDecode:
    {
    break;
    }
  default:
    {
      printf("Invalid fetch state\n");
      break;
    }
  }
}

void FunctionalCPU__tick (FunctionalCPU * const this) {
  this->super->tick(this);
  printf("Functional CPU ticked\n");
  FunctionalCPU__fetch(this);
}

void FunctionalCPU__setMemoryQueues(FunctionalCPU* const this, Queue* const toMemory, Queue* const fromMemory) {
  this->toMemory = toMemory;
  this->fromMemory = fromMemory;
}

void FunctionalCPU__setPC(FunctionalCPU * const this, uint64_t const pc) {
  this->pc = pc;
}

static const struct FunctionalCPU_Vtbl FunctionalCPU_Vtbl =
{
 .tick = FunctionalCPU__tick,
 .setPC = FunctionalCPU__setPC,
 .setMemoryQueues = FunctionalCPU__setMemoryQueues,
};

void FunctionalCPU__ctor(FunctionalCPU * const this) {
  CPU__ctor(this);
  this->super = this->vptr;
  this->vptr = &FunctionalCPU_Vtbl;

  this->pc = 0;
  this->toMemory = nullptr;
  this->fromMemory = nullptr;

  this->fetchState = 0;
}

void FunctionalCPU__dtor(FunctionalCPU * const this) {
  CPU__dtor(this);
}
