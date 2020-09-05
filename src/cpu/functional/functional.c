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

void CPU_fetch(CPU * const cpu) {
  switch(cpu->fetchState) {
  case RequestToMemoryState:
    {
      Request* req = (Request*)malloc(sizeof(Request));
      Request_init(req);
      Request_setDataSize(req, 4);
      Request_setAddress(req, cpu->pc);
      Queue_push(cpu->toMemory, req);
      printf("Sent pc %lx request to mem\n", cpu->pc);

      cpu->fetchBytes = 0;
      cpu->fetchState = WaitingToMemoryState;
      break;
    }
  case WaitingToMemoryState:
    {
      Request* req = Queue_pop(cpu->fromMemory);
      if (req != nullptr) {
	uint64_t addr = Request_getAddress(req);
	printf("Address %lx answered from memory\n", addr);

	uint8_t i = 0;
	for (i = 0; i < Request_getDataSize(req); ++i) {
	  uint8_t byte = Request_getData(req)[i];
	  printf("0x%02x ", byte);
	  cpu->fetchBytes |= ((uint32_t)byte) << (i * 8);
	}
	printf("\n");
	/*printf("FetchBytes: %08lx\n", cpu->fetchBytes);*/

	free(Request_getData(req));
	Request_destroy(req);
	delete(req);

	cpu->currentFetchByte = 0;
	cpu->fetchState = SendToDecodeState;
      }
      break;
    }
  case SendToDecodeState:
    {
      if  (QueueU8_isFull(cpu->decodeBytes)) break;
      uint8_t byte = cpu->fetchBytes >> (cpu->currentFetchByte * 8);
      QueueU8_push(cpu->decodeBytes, byte);
      /* printf("Push %02x in the queue\n", byte); */
      cpu->currentFetchByte++;
      if(cpu->currentFetchByte == 4) {
	cpu->pc += 4;
	cpu->fetchState = RequestToMemoryState;
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
		 RX, /* Rex */
		 LK, /* Lock */
		 SZ, /* Size */
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
   /* 6 */ 0, 0, 0, 0, 0, 0, 0,SZ, 0, 0, 0, 0, 0, 0, 0, 0,
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

const uint8_t modRMTable[][0x100] = {
  /* One Byte Opcode */
  {
        /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   /* 0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 2 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 3 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 4 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 5 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 6 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 7 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 8 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
   /* 9 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* A */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* B */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* C */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* D */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* E */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  },

  /* Two Bytes Opcode */
  {
        /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   /* 0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 2 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 3 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 4 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
   /* F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  },

  /* Three Bytes Opcode */
  {
        /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   /* 0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 2 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 3 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 4 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
   /* F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  }
};

const uint8_t sibTable[] =
  {
        /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   /* 0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 2 */ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 3 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 4 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 5 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 6 */ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 7 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 8 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 9 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* A */ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
   /* B */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* C */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* D */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* E */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

enum dispSize {
  D1 = 1,
  D4 = 4
};

const uint8_t dispTable[] =
  {
        /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   /* 0 */ 0, 0, 0, 0, 0,D4, 0, 0, 0, 0, 0, 0, 0,D4, 0, 0,
   /* 1 */ 0, 0, 0, 0, 0,D4, 0, 0, 0, 0, 0, 0, 0,D4, 0, 0,
   /* 2 */ 0, 0, 0, 0, 0,D4, 0, 0,D4,D4,D4,D4,D4,D4,D4,D4,
   /* 3 */ 0, 0, 0, 0, 0,D4, 0, 0, 0, 0, 0, 0, 0,D4, 0, 0,
   /* 4 */D1,D1,D1,D1,D1,D1,D1,D1,D1,D1,D1,D1,D1,D1,D1,D1,
   /* 5 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 6 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 7 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 8 */D4,D4,D4,D4,D4,D4,D4,D4,D4,D4,D4,D4,D4,D4,D4,D4,
   /* 9 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* A */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* B */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* C */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* D */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* E */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

enum immSize {
  I1 = 1,
  I2 = 2,
  I4 = 4
};

const uint8_t immediateTable[][0x100] = {
  /* One Byte Opcode */
  {
        /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   /* 0 */ 0, 0, 0, 0, 0,I2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 2 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 3 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 4 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 5 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 6 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 7 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 8 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 9 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* A */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* B */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* C */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* D */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* E */ 0, 0, 0, 0, 0, 0, 0, 0,I4, 0, 0,I1, 0, 0, 0, 0,
   /* F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  },

  /* Two Bytes Opcode */
  {
        /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   /* 0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 2 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 3 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 4 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
   /* F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  },

  /* Three Bytes Opcode */
  {
        /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   /* 0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 2 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 3 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   /* 4 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
   /* F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  }
};

enum OpcodeType
  {
   OneByte = 0,
   TwoBytes,
   ThreeBytes,
  };

void CPU_decode(CPU * const cpu) {
  if (cpu->decodeState == ResetState) {
    memset(&cpu->decodedInst, 0, sizeof(X86Inst));
    cpu->decodeState = PrefixState;
    cpu->dispRead = 0;
    cpu->immRead = 0;
  }

  if  (QueueU8_isEmpty(cpu->decodeBytes)) return;
  uint8_t byte = QueueU8_front(cpu->decodeBytes);
  /*printf("Decoded found byte %02x\n", byte);*/

  switch(cpu->decodeState) {
  case PrefixState:
    {
      switch(prefixTable[byte]) {

      case RX:
	{
	  uint8_t byte = QueueU8_pop(cpu->decodeBytes);
	  cpu->decodedInst.prefix.rex = byte;
	  /*printf("Found REX prefix %02x\n", byte);

	  printf("\tW = %02x\n", cpu->decodedInst.prefix.W);
	  printf("\tR = %02x\n", cpu->decodedInst.prefix.R);
	  printf("\tX = %02x\n", cpu->decodedInst.prefix.X);
	  printf("\tB = %02x\n", cpu->decodedInst.prefix.B);*/
	  break;
	}

      case SZ:
	{
	  uint8_t byte = QueueU8_pop(cpu->decodeBytes);
	  cpu->decodedInst.prefix.opsize = 1;
	  /*printf("Found Opsize prefix %02x\n", byte);*/
	  break;
	}

      case LK:
	{
	  uint8_t byte = QueueU8_pop(cpu->decodeBytes);
	  cpu->decodedInst.prefix.lock = 1;
	  /*printf("Found LOCK prefix %02x\n", byte);*/
	  break;
	}

	/* Missing more prefixes */

      case NONE:
	{
	  cpu->decodeState = Opcode1ByteState;
	  break;
	}
      }
      break;
    }
  case Opcode1ByteState:
    {
      uint8_t byte = QueueU8_pop(cpu->decodeBytes);
      if (byte == 0x0F) {
	/*printf("Found 0x0F, Two Bytes opcode\n");*/
	cpu->decodeState = Opcode2BytesState;
	break;
      }
      
      cpu->decodedInst.type = OneByte;
      cpu->decodedInst.opcode = byte;
      /*printf("Found %02x opcode\n", byte);*/

      cpu->decodeState = ModRMState;      
      
      break;
    }
  case ModRMState:
    {
      if(!modRMTable[cpu->decodedInst.type][cpu->decodedInst.opcode]) {
	cpu->decodeState = ImmediateState;
	break;
      }

      uint8_t byte = QueueU8_pop(cpu->decodeBytes);
      cpu->decodedInst.modRM = byte;
      /*printf("Found %02x modRM\n", byte);

      printf("\tRM = %02x\n", cpu->decodedInst.RM);
      printf("\tReg = %02x\n", cpu->decodedInst.reg);
      printf("\tMod = %02x\n", cpu->decodedInst.mod);*/

      if (cpu->decodedInst.mod == 0x00 &&
	  cpu->decodedInst.RM == 0x05) {
	cpu->decodedInst.rip = 1;
	cpu->decodeState = DisplacementState;
	/*printf("Rip Mode Addressing\n");*/
	break;
      }
      
      cpu->decodeState = SIBState;
    }
  case SIBState:
    {
      if (!sibTable[cpu->decodedInst.modRM]){
	cpu->decodeState = ImmediateState;
	break;
      }

      uint8_t byte = QueueU8_pop(cpu->decodeBytes);
      cpu->decodedInst.sib = byte;
      /* printf("Found %02x sib\n", byte);

      printf("\tBase = %02x\n", cpu->decodedInst.base);
      printf("\tIndex = %02x\n", cpu->decodedInst.index);
      printf("\tSS = %02x\n", cpu->decodedInst.scale); */
	   
      cpu->decodeState = DisplacementState;
      break;
    }
  case DisplacementState:
    {
      uint8_t bytesToRead = dispTable[cpu->decodedInst.modRM];
      if (!bytesToRead){
	cpu->decodeState = ImmediateState;
	break;
      }
      
      cpu->decodedInst.disp |= (((uint32_t)QueueU8_pop(cpu->decodeBytes)) << (8 * cpu->dispRead++));
      /* printf("Found %08x disp\n", cpu->decodedInst.disp); */

      if (cpu->dispRead == bytesToRead)
	cpu->decodeState = ImmediateState;
      break;
    }
  case ImmediateState:
    {
      uint8_t bytesToRead = immediateTable[cpu->decodedInst.type][cpu->decodedInst.opcode];
      if (!bytesToRead){
	cpu->decodeState = SendToMicroDispatchState;
	break;
      }

      if (cpu->decodedInst.prefix.W && bytesToRead == 2) bytesToRead = 4;
      
      cpu->decodedInst.imm |= (((uint32_t)QueueU8_pop(cpu->decodeBytes)) << (8 * cpu->immRead++));
      /* printf("Found %08lx imm\n", cpu->decodedInst.imm); */

      if (cpu->immRead == bytesToRead)
	cpu->decodeState = SendToMicroDispatchState;
      break;
    }
  case SendToMicroDispatchState:
    {
      printf("------------------\n");
      printf("Prefixes\n");
      printf(" opsize = %x\n", cpu->decodedInst.prefix.opsize);
      printf(" lock = %x\n", cpu->decodedInst.prefix.lock);
      printf(" rex = %x\n", cpu->decodedInst.prefix.rex);
      printf("  rip = %x\n", cpu->decodedInst.rip);
      printf("Opcode = %x\n", cpu->decodedInst.opcode);
      printf("ModRM = %x\n", cpu->decodedInst.modRM);
      printf("SIB = %x\n", cpu->decodedInst.sib);
      printf("DISP = %x\n", cpu->decodedInst.disp);
      printf("IMM = %lx\n", cpu->decodedInst.imm);
      printf("------------------\n");
      
      cpu->decodeState = ResetState;
      break;
    }
    
  }
}

void CPU_tick (CPU * const cpu) {
  CPU_fetch(cpu);
  CPU_decode(cpu);
}

void CPU_setMemoryQueues(CPU * const cpu, Queue* const toMemory, Queue* const fromMemory) {
  cpu->toMemory = toMemory;
  cpu->fromMemory = fromMemory;
}

void CPU_setPC(CPU * const cpu, uint64_t const pc) {
  cpu->pc = pc;
}

void CPU_init(CPU * const cpu) {
  cpu->pc = 0;
  cpu->toMemory = nullptr;
  cpu->fromMemory = nullptr;

  cpu->fetchState = 0;
  cpu->fetchBytes = 0;
  cpu->currentFetchByte = 0;

  cpu->decodeBytes = (QueueU8*)malloc(sizeof(QueueU8));
  QueueU8_init(cpu->decodeBytes, 64);
  cpu->decodeState = PrefixState;
  memset(&cpu->decodedInst, 0, sizeof(X86Inst));
}

void CPU_destroy(CPU * const cpu) {
  QueueU8_destroy(cpu->decodeBytes);
  delete(cpu->decodeBytes);
}
