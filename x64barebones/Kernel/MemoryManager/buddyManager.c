#include "../include/memoryManager.h"

#define MIN_BLOCK_SIZE 1024
#define MAX_2_POW 18
#define BLOCK_SIZE(i) ((uint64_t) (i << (i)) * MIN_BLOCK_SIZE)
#define GET_BUDDY(b, i) ((((uint64_t)(b)) ^ (BLOCK_SIZE(i))))

typedef struct Block {
    uint64_t size;
    Block* next;
} Block;

static Block* free_lists[MAX_2_POW +1];
static void* start_heap;
MemoryStatus mem_status;

void my_mm_init(void* ptr) {



    for(int i=0; i < MAX_2_POW; i++) {
        free_lists[i] = NULL;
    }


}






