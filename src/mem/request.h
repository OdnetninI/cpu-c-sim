/*
request.h - Request class to memory hierarchy
 */
#ifndef __Request_H__
#define __Request_H__

#pragma once

#include "../common.h"

/* Attributes of the class */
typedef struct _request {
  uint64_t address;
  
  uint64_t dataSize;
  uint8_t* data;
} Request;

/* Constructors */
void Request_init(Request * const request);
void Request_destroy(Request * const request);

void Request_setAddress(Request * const request, uint64_t address);
uint64_t Request_getAddress(Request const * const request);
void Request_setDataSize(Request * const request, uint64_t dataSize);
uint64_t Request_getDataSize(Request * const request);
void Request_setData(Request * const request, uint8_t* data);
uint8_t* Request_getData(Request * const request);

#endif /* __Request_H__ */
