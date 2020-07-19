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

/* Request Class Attributes */
struct Request_Vtbl; /* Forward declaration */
typedef struct __request {
  struct Request_Vtbl const *vptr; /* Virtual Function Table */

  uint64_t address;
} Request;

/* Request Class Virtual Function Table */
typedef struct Request_Vtbl {
  void (*setAddress)(Request * const this, uint64_t address);
  uint64_t (*getAddress)(Request const * const this);
} _Request_Vtbl;

/* Constructors */
void Request__ctor(Request * const this);
void Request__dtor(Request * const this);

#endif /* __Request_H__ */
