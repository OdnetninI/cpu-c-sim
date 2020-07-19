/*
request.h - Request class to memory hierarchy
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
#ifndef __Request_H__
#define __Request_H__

#pragma once

#include "../common.h"

/* Attributes of the class */
typedef struct Request_Data {
  uint64_t address;
  
  uint64_t dataSize;
  uint8_t* data;
} _Request_Data;

/* Request Class */
struct Request_Vtbl; /* Forward declaration */
typedef struct __request {
  struct Request_Vtbl const *vptr; /* Virtual Function Table */
  void const *super; /* Parent Virtual Function Table */
  _Request_Data; /* Attributes of the class (Anonymous struct) */
} Request;

/* Request Class Virtual Function Table */
typedef struct Request_Vtbl {
  void (*setAddress)(Request * const this, uint64_t address);
  uint64_t (*getAddress)(Request const * const this);

  void (*setDataSize)(Request * const this, uint64_t dataSize);
  uint64_t (*getDataSize)(Request * const this);
  void (*setData)(Request * const this, uint8_t* data);
  uint8_t* (*getData)(Request * const this);
} _Request_Vtbl;

/* Constructors */
void Request__ctor(Request * const this);
void Request__dtor(Request * const this);

#endif /* __Request_H__ */
