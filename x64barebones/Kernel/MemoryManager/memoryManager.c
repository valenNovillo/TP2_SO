#include "../include/memoryManager.h"
#include "../include/lib.h"
#include "../Drivers/include/videoDriver.h"
#include "../Drivers/include/fileDescriptors.h"

#define BLOCK_COUNT 8192
#define BLOCK_SIZE 2048

static int memory_size;
static int current_block;
static void* free_ptrs[BLOCK_COUNT];
static MemoryStatus mem_status;

void my_mm_init(void* ptr) {
    current_block = 0;
    memory_size = BLOCK_COUNT * BLOCK_SIZE;
    for(int i = 0; i < BLOCK_COUNT; i++) {
        free_ptrs[i] = ptr + BLOCK_SIZE * i;
    }
    mem_status.total = memory_size;
    mem_status.reserved = 0;
    mem_status.free = memory_size;
}

void* my_malloc(uint64_t size) {
    _cli();
    if (size <= BLOCK_SIZE && current_block < BLOCK_COUNT) {
        _sti();
        mem_status.reserved += BLOCK_SIZE;
        mem_status.free -= BLOCK_SIZE;
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
        mem_status.reserved -= BLOCK_SIZE;
        mem_status.free += BLOCK_SIZE;
    }
    _sti();
}

void print_mem_status() {
    int len;

    putString(STDOUT, "\nMemory status:\n", 16);
    
    putString(STDOUT, "TOTAL: ", 7);
    char string_total[10];
    len = intToString(mem_status.total, string_total);
    putString(STDOUT, string_total, len);

    putString(STDOUT, "\nRESERVED: ", 11);
    char string_reserved[10];
    len = intToString(mem_status.reserved, string_reserved);
    putString(STDOUT, string_reserved, len);

    putString(STDOUT, "\nFREE: ", 7);
    char string_free[10];
    len = intToString(mem_status.free, string_free);
    putString(STDOUT, string_free, len);

    putString(STDOUT, "\n", 1);
}











