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

void FunctionalCPU__tick (FunctionalCPU const * const this) {
  this->super->tick(this);
  printf("Functional CPU ticked\n");
  Request* req = new(Request);
  req->vptr->setAddress(req, this->pc);
  this->toMemory->vptr->push(this->toMemory, req);
  printf("Sent pc %x request to mem\n", this->pc);
}

void FunctionalCPU__setMemoryQueues(FunctionalCPU* const this, Queue* const toMemory, Queue* const fromMemory) {
  this->toMemory = toMemory;
  this->fromMemory = fromMemory;
}

static const struct FunctionalCPU_Vtbl FunctionalCPU_Vtbl =
{
 .tick = FunctionalCPU__tick,
 .setMemoryQueues = FunctionalCPU__setMemoryQueues,
};

void FunctionalCPU__ctor(FunctionalCPU * const this) {
  CPU__ctor(this);
  this->super = this->vptr;
  this->vptr = &FunctionalCPU_Vtbl;

  this->pc = 0;
  this->toMemory = nullptr;
  this->fromMemory = nullptr;
}

void FunctionalCPU__dtor(FunctionalCPU * const this) {
  CPU__dtor(this);
}
