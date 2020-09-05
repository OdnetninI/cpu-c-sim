/*
request.c - Request class to memory hierarchy
 */
#include "../common.h"
#include "request.h"

void Request_setAddress(Request * const request, uint64_t address) {
  request->address = address;
}

uint64_t Request_getAddress(Request const * const request) {
  return request->address;
}

void Request_setDataSize(Request * const request, uint64_t dataSize) {
  request->dataSize = dataSize;
}

uint64_t Request_getDataSize(Request * const request) {
  return request->dataSize;
}

void Request_setData(Request * const request, uint8_t* data) {
  request->data = data;
}

uint8_t* Request_getData(Request * const request) {
  return request->data;
}

void Request_init(Request * const request) {
  request->address = 0x00;
}

void Request_destroy(Request * const request) {
  
}
