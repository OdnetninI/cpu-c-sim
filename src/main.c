#include <stdio.h>
#include "cpu/functional/functional.h"
#include "mem/simple/simple.h"
#include "base/queue.h"

#include <elf.h>

int main (int argc, char* argv[], char* envp[]) {
  printf("Hello World!\n");

  CPU* cpu = (CPU*)malloc(sizeof (CPU));
  Mem* mem = (Mem*)malloc(sizeof (Mem));
  CPU_init(cpu);
  Mem_init(mem);

  Queue* cpuToMem = (Queue*)malloc(sizeof (Queue));
  Queue* memToCpu = (Queue*)malloc(sizeof (Queue));
  Queue_init(cpuToMem, 32);
  Queue_init(memToCpu, 32);

  CPU_setMemoryQueues(cpu, cpuToMem, memToCpu);
  Mem_setCPUQueues(mem, cpuToMem, memToCpu);


  /* Read Linux binary */
  printf("-------------------------\n");
  printf("Loading Kernel ELF Binary\n");
  printf("-------------------------\n");
  FILE* file = fopen(argv[1], "rb");
  if (!file) return 1;

  Elf64_Ehdr header;
  fread(&header, 1, sizeof(header), file);
  if (memcmp(header.e_ident, ELFMAG, SELFMAG)) return 2;

  /*printf("Entry point %x\n", header.e_entry);
  printf("Programs headers offset %d\n", header.e_phoff);
  printf("Programs headers size %d\n", header.e_phentsize);
  printf("Programs headers entries %d\n", header.e_phnum);*/

  CPU_setPC(cpu, header.e_entry);

  fseek(file, header.e_phoff, SEEK_SET);

  Elf64_Phdr* programs = malloc(header.e_phentsize * header.e_phnum);
  fread(programs, header.e_phentsize, header.e_phnum, file);

  int p = 0;
  /*printf("Programs\n");*/
  for (p = 0; p < header.e_phnum; ++p) {
    Elf64_Phdr program = programs[p];
    if (program.p_type != 1) continue;
    /*printf("Type: %x ", program.p_type);
    printf("File Offset: %08x ", program.p_offset);
    printf("File Size: %08x ", program.p_filesz);
    printf("PhysAddr: %08x ", program.p_paddr);
    printf("Align: %08x\n", program.p_align);*/

    fseek(file, program.p_offset, SEEK_SET);
    uint8_t* data = malloc(program.p_filesz);
    fread(data, 1, program.p_filesz, file);
    Mem_initializeMemory(mem, program.p_paddr, data, program.p_filesz);
    free(data);
  }

  free(programs);
  fclose(file);
  
  int i = 0;
  for (i = 0; i < 10; ++i) {
    CPU_tick(cpu);
    if (i%2 == 0) Mem_tick(mem);
  }

  Mem_destroy(mem);
  Queue_destroy(cpuToMem);
  Queue_destroy(memToCpu);
  delete(cpu);
  delete(mem);
  delete(cpuToMem);
  delete(memToCpu);
  
  return 0;
}
