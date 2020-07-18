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

/* CPU Class Attributes */
struct CPU_Vtbl; /* Forward declaration */
typedef struct __cpu {
  struct CPU_Vtbl const *vptr; /* Virtual Function Table */
  
} CPU;

/* CPU Class Virtual Function Table */
struct CPU_Vtbl {
  void (*test)(CPU const * const this);
};

/* Constructors */
void CPU__ctor(CPU * const this);
void CPU__dtor(CPU * const this);

#endif /* __CPU_H__ */
