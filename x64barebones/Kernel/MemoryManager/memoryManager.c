#include "../include/memoryManager.h"

void* start;
int memory_size;
int current;
void* free_ptrs[BLOCK_COUNT];

void my_mm_init(void* ptr) {
    start = ptr;
    current = 0;
    memory_size = BLOCK_COUNT * BLOCK_SIZE;
    for(int i = 0; i < BLOCK_COUNT; i++) {
        free_ptrs[i] = start + BLOCK_SIZE * i;
    }
}

void* my_malloc(uint64_t size) {
    if (size <= BLOCK_SIZE && current < BLOCK_COUNT) {
        return free_ptrs[current++];
    } else {
        return NULL;
    }
}

void my_free(void * ptr) {
    free_ptrs[--current] = ptr;
}











