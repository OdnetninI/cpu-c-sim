#include <stdio.h>
#include "cpu/functional/functional.h"

int main (int argc, char* argv[], char* envp[]) {
  printf("Hello World!\n");

  FunctionalCPU cpu;
  FunctionalCPU__ctor(&cpu);

  cpu.vptr->tick(&cpu);

  FunctionalCPU__dtor(&cpu);
  
  return 0;
}
