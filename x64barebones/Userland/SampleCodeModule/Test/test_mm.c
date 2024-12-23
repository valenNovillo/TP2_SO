// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdlib.h>
#include "test_util.h"
#include "test.h"
#include "../Library/include/stdio1.h"
#include "../Library/include/unistd1.h"
#include "../Library/include/lib.h"

#ifdef BUDDY_MODE 
  #define MAX_BLOCKS 128
#else
  #define MAX_BLOCKS 128
  #define BLOCK_SIZE 2048
#endif

typedef struct MM_rq{
  void *address;
  uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[]){

  mm_rq mm_rqs[MAX_BLOCKS];
  uint64_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 1){
    return -1;
  }

  if ((max_memory = satoi(argv[0])) <= 0){
    return -1;
  }

  int j = 0;

  while (1){
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    #ifdef BUDDY_MODE 
      while (rq < MAX_BLOCKS && total < max_memory){
        mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
        mm_rqs[rq].address = my_malloc(mm_rqs[rq].size);

        if (mm_rqs[rq].address){
          total += mm_rqs[rq].size;
          rq++;
        }
      }
    #else
      while (rq < MAX_BLOCKS){
        mm_rqs[rq].size = GetUniform(BLOCK_SIZE - 1) + 1;
        mm_rqs[rq].address = my_malloc(mm_rqs[rq].size);

        if (mm_rqs[rq].address){
          total += mm_rqs[rq].size;
          rq++;
        }
      }
    #endif
    
    // Set
    //printf("Sali\n");
    if((j % 5) == 0) {
      printf("\nReserva\n");
      print_mem_status(get_fds());
    }
    
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)){
          printf("test_mm ERROR\n");
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        my_free(mm_rqs[i].address);

    

    if((j % 5) == 0) {
      printf("\nLiberacion\n");
      print_mem_status(get_fds());
    }

    j++;
  }
}
