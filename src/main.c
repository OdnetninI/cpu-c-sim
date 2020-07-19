#include <stdio.h>
#include "cpu/functional/functional.h"
#include "mem/simple/simple.h"
#include "base/queue.h"

int main (int argc, char* argv[], char* envp[]) {
  printf("Hello World!\n");

  FunctionalCPU* cpu = new(FunctionalCPU);
  SimpleMem* mem = new(SimpleMem);

  Queue* cpuToMem = new(Queue, 32);
  Queue* memToCpu = new(Queue, 32);

  cpu->vptr->setMemoryQueues(cpu, cpuToMem, memToCpu);
  mem->vptr->setCPUQueues(mem, cpuToMem, memToCpu);
  
  int i = 0;
  for (i = 0; i < 10; ++i) {
    cpu->vptr->tick(cpu);
    if (i%2 == 0) mem->vptr->tick(mem);
  }

  FunctionalCPU__dtor(cpu);
  SimpleMem__dtor(mem);
  Queue__dtor(cpuToMem);
  Queue__dtor(memToCpu);
  delete(cpu);
  delete(mem);
  delete(cpuToMem);
  delete(memToCpu);
  
  return 0;
}
