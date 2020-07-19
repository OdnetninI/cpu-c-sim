/*
mem.h - Base Memory class
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
#ifndef __MEM_H__
#define __MEM_H__

#pragma once

#include "../common.h"

/* Mem Class Attributes */
struct Mem_Vtbl; /* Forward declaration */
typedef struct __mem {
  struct Mem_Vtbl const *vptr; /* Virtual Function Table */
  
} Mem;

/* Mem Class Virtual Function Table */
typedef struct Mem_Vtbl {
  void (*tick)(Mem const * const this);
} _Mem_Vtbl;

/* Constructors */
void Mem__ctor(Mem * const this);
void Mem__dtor(Mem * const this);

#endif /* __MEM_H__ */
