/*
request.c - Request class to memory hierarchy
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
#include "../common.h"
#include "request.h"

void Request__setAddress(Request * const this, uint64_t address) {
  this->address = address;
}

uint64_t Request__getAddress(Request const * const this) {
  return this->address;
}

void Request__setDataSize(Request * const this, uint64_t dataSize) {
  this->dataSize = dataSize;
}

uint64_t Request__getDataSize(Request * const this) {
  return this->dataSize;
}

void Request__setData(Request * const this, uint8_t* data) {
  this->data = data;
}

uint8_t* Request__getData(Request * const this) {
  return this->data;
}

static const struct Request_Vtbl Request_Vtbl =
{
 .setAddress = Request__setAddress,
 .getAddress = Request__getAddress,
 
 .setDataSize = Request__setDataSize,
 .getDataSize = Request__getDataSize,
 .setData = Request__setData,
 .getData = Request__getData,
};

void Request__ctor(Request * const this) {
  this->vptr = &Request_Vtbl;
  this->address = 0x00;
}

void Request__dtor(Request * const this) {
  
}
