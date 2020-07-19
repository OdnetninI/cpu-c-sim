/*
mem.c - Base Memory class
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
#include "mem.h"

void Mem__tick (Mem const * const this) {
  printf("Mem ticked\n");
}

static const struct Mem_Vtbl Mem_Vtbl =
{
 .tick = Mem__tick,
 
};

void Mem__ctor(Mem * const this) {
  this->vptr = &Mem_Vtbl;
}

void Mem__dtor(Mem * const this) {
  
}
