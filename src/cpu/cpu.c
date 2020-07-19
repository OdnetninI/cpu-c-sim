/*
cpu.c - Base CPU class
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
#include "cpu.h"

void CPU__tick (CPU const * const this) {
  printf("CPU ticked\n");
}

static const struct CPU_Vtbl CPU_Vtbl =
{
 .tick = CPU__tick,
 .setPC = nullptr,
};

void CPU__ctor(CPU * const this) {
  this->vptr = &CPU_Vtbl;
  this->super = nullptr;
}

void CPU__dtor(CPU * const this) {
  
}
