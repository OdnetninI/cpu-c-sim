#include <stdio.h>
#include "cpu/cpu.h"

int main (int argc, char* argv[], char* envp[]) {
  printf("Hello World!\n");

  CPU cpu;
  CPU__ctor(&cpu);

  cpu.vptr->test(&cpu);

  CPU__dtor(&cpu);
  
  return 0;
}
