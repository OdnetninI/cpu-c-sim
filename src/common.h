/*
common.h - Common C includes for all files
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
#ifndef __COMMON_H__
#define __COMMON_H__

#pragma once

/* Common C Includes */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* Class defines */
#define new(x, ...) ({x* tmp = (x*)malloc(sizeof(x)); x##__ctor(tmp , ##__VA_ARGS__); tmp;})
#define delete(x) {free(x);};

/* Common Defines */
#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)
#define GB(x) ((x) << 30)
#define TB(x) ((x) << 40)

#define nullptr ((void*)0x00)
#define null (0x00)

#endif /* __COMMON_H__ */
