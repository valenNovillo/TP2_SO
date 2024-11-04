#include "../include/memoryManager.h"

#define BLOCK_COUNT 8192
#define BLOCK_SIZE 2048
#define MEMORY_SIZE "16777216"

void* start_heap;
int memory_size;
int current_block;
void* free_ptrs[BLOCK_COUNT];

void my_mm_init(void* ptr) {
    start_heap = ptr;
    current_block = 0;
    memory_size = BLOCK_COUNT * BLOCK_SIZE;
    for(int i = 0; i < BLOCK_COUNT; i++) {
        free_ptrs[i] = start_heap + BLOCK_SIZE * i;
    }
}

void* my_malloc(uint64_t size) {
    _cli();
    if (size <= BLOCK_SIZE && current_block < BLOCK_COUNT) {
        _sti();
        return free_ptrs[current_block++];
    } else {
        _sti();
        return NULL;
    }
}

void my_free(void * ptr) {
    _cli();
    if (current_block > 0) {
        free_ptrs[--current_block] = ptr;
    }
    _sti();
}











