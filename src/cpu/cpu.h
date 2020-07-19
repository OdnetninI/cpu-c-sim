/*
cpu.h - Base CPU class
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
#ifndef __CPU_H__
#define __CPU_H__

#pragma once

#include "../common.h"

/* Attributes of the class */
typedef struct CPU_Data {
  
} _CPU_Data;

/* CPU Class */
struct CPU_Vtbl;
typedef struct __cpu {
  struct CPU_Vtbl const *vptr; /* Virtual Function Table */
  void const * super; /* Parent Virtual Function Table */
  _CPU_Data; /* Attributes of the class (Anonymous struct) */
} CPU;

/* CPU Class Virtual Function Table */
typedef struct CPU_Vtbl {
  void (*tick)(CPU * const this);
} _CPU_Vtbl;

/* Constructors */
void CPU__ctor(CPU * const this);
void CPU__dtor(CPU * const this);

#endif /* __CPU_H__ */
