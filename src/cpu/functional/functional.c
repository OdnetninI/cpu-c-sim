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
		 RequestToMemoryState = 0,
		 WaitingToMemoryState,
		 SendToDecodeState
};

enum DecodeState {
		 PrefixState = 0,
		 Opcode1ByteState,
		 Opcode2BytesState,
		 Opcode3BytesAState,
		 Opcode3BytesBState,
		 ModRMState,
		 SIBState,
		 DisplacementState,
		 ImmediateState,
		 SendToMicroDispatchState,
		 ResetState,
};

void FunctionalCPU__fetch(FunctionalCPU * const this) {
  switch(this->fetchState) {
  case RequestToMemoryState:
    {
      Request* req = new(Request);
      req->vptr->setDataSize(req, 4);
      req->vptr->setAddress(req, this->pc);
      this->toMemory->vptr->push(this->toMemory, req);
      printf("Sent pc %x request to mem\n", this->pc);
    
      this->fetchState = WaitingToMemoryState;
      break;
    }
  case WaitingToMemoryState:
    {
      Request* req = this->fromMemory->vptr->pop(this->fromMemory);
      if (req != nullptr) {
	uint64_t addr = req->vptr->getAddress(req);
	printf("Address %x answered from memory\n", addr);

	int i = 0;
	for (i = 0; i < req->vptr->getDataSize(req); ++i) {
	  printf("0x%02x ", req->vptr->getData(req)[i]);
	  this->fetchBytes |= (req->vptr->getData(req)[i]) << (i * 8);
	}
	printf("\n");

	free(req->vptr->getData(req));
	Request__dtor(req);
	delete(req);

	this->currentFetchByte = 0;
	this->fetchState = SendToDecodeState;
      }
      break;
    }
  case SendToDecodeState:
    {
      if  (this->decodeBytes->vptr->isFull(this->decodeBytes)) break;
      uint8_t byte = this->fetchBytes >> (this->currentFetchByte * 8);
      this->decodeBytes->vptr->push(this->decodeBytes, byte);
      this->currentFetchByte++;
      if(this->currentFetchByte == 4) {
	this->pc += 4;
	this->fetchState = RequestToMemoryState;
      }
    break;
    }
  default:
    {
      printf("Invalid fetch state\n");
      break;
    }
  }
}

enum Prefixes {
		 NONE = 0,
		 RX,
		 LK,
};

const uint8_t prefixTable[] =
  {
        /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   /* 0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 2 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 3 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 4 */RX,RX,RX,RX,RX,RX,RX,RX,RX,RX,RX,RX,RX,RX,RX,RX,
   /* 5 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 6 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 7 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 8 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 9 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* A */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* B */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* C */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* D */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* E */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* F */LK, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

enum OpcodeType
  {
   OneByte = 0,
   TwoBytes,
   ThreeBytes,
  };

void FunctionalCPU__decode(FunctionalCPU * const this) {
  if (this->decodeState == ResetState) {
    memset(&this->decodedInst, 0, sizeof(X86Inst));
    this->decodeState = PrefixState;
  }

  if  (this->decodeBytes->vptr->isEmpty(this->decodeBytes)) return;
  uint8_t byte = this->decodeBytes->vptr->front(this->decodeBytes);
  printf("Decoded found byte %02x\n", byte);

  switch(this->decodeState) {
  case PrefixState:
    {
      switch(prefixTable[byte]) {

      case RX:
	{
	  uint8_t byte = this->decodeBytes->vptr->pop(this->decodeBytes);
	  this->decodedInst.prefix.rex = byte;
	  printf("Found REX prefix %02x\n", byte);
	  break;
	}

      case LK:
	{
	  uint8_t byte = this->decodeBytes->vptr->pop(this->decodeBytes);
	  this->decodedInst.prefix.lock = 1;
	  printf("Found LOCK prefix %02x\n", byte);
	  break;
	}

	/* Missing more prefixes */

      case NONE:
	{
	  this->decodeState = Opcode1ByteState;
	  break;
	}
      }
      break;
    }
  case Opcode1ByteState:
    {
      uint8_t byte = this->decodeBytes->vptr->pop(this->decodeBytes);
      if (byte == 0x0F) {
	printf("Found 0x0F, Two Bytes opcode\n");
	this->decodeState = Opcode2BytesState;
	break;
      }
      
      this->decodedInst.type = OneByte;
      this->decodedInst.opcode = byte;
      printf("Found %02x opcode\n", byte);

      /* Now jumpt to Immediate or ModRM depeding on the opcode itself */
      this->decodeState = ModRMState;
      
      break;
    }
  }
}

void FunctionalCPU__tick (FunctionalCPU * const this) {
  this->super->tick(this);
  FunctionalCPU__fetch(this);
  FunctionalCPU__decode(this);
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
  this->fetchBytes = 0;
  this->currentFetchByte = 0;

  this->decodeBytes = new(QueueU8, 64);
  this->decodeState = PrefixState;
  memset(&this->decodedInst, 0, sizeof(X86Inst));
}

void FunctionalCPU__dtor(FunctionalCPU * const this) {
  QueueU8__dtor(this->decodeBytes);
  delete(this->decodeBytes);
  
  CPU__dtor(this);
}
