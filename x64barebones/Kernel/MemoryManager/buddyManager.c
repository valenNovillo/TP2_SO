#include "../include/memoryManager.h"
#include "../include/lib.h"
#include "../Drivers/include/videoDriver.h"
#include "../Drivers/include/fileDescriptors.h"

#define MIN_BLOCK_SIZE 1024
#define MAX_2_POW 19

typedef struct Block {
    uint64_t size;
    struct Block* next;
} Block;

static Block* free_lists[MAX_2_POW +1];
static MemoryStatus mem_status;
static void* start_heap;

static uint64_t block_size(int i) {
    return (uint64_t) ((i << i) * MIN_BLOCK_SIZE);
}

static uint64_t get_buddy(int b, int i) {
    return (uint64_t) (b ^ block_size(i)); 
}

static int get_index(uint64_t size){
    int i = 0;
    while(block_size(i) < size && i < MAX_2_POW){
        i++;
    }

    return i;
}

void my_mb_init(void* ptr, uint64_t size) {
    if(size < MIN_BLOCK_SIZE)
        return;

    for(int i=0; i < MAX_2_POW + 1; i++) {
        free_lists[i] = NULL;
    }
    
    int i = get_index(size);

    start_heap = ptr;
    free_lists[i] = ptr;
    free_lists[i]->next = NULL;
    free_lists[i]->size = block_size(i);
    mem_status.total = free_lists[i]->size;
    mem_status.free = free_lists[i]->size;
    mem_status.reserved = 0;
}

static Block* my_malloc_rec(uint64_t size){
    if (size > block_size(MAX_2_POW)) {
        return NULL;
    }
    
    int i = get_index(size);

    if(free_lists[i] != NULL){
        Block* block = free_lists[i];
        free_lists[i] = free_lists[i]->next;
        block->size = block_size(i);
        mem_status.reserved += block->size;
        mem_status.free -= block->size;
        return block;
    }

    Block* block = my_malloc_rec(block_size(i + 1));
    
    if(block != NULL){
        uint64_t address_diff_from_base = (void*)block - start_heap;
        Block *buddy = (Block *)(get_buddy(address_diff_from_base, i) + (uint64_t)start_heap);

        buddy->size = block_size(i);
        buddy->next = free_lists[i];
        free_lists[i] = buddy;

        block->size = block_size(i);
    }

    return block;
}


void * my_malloc(uint64_t size) {
    if(size == 0) {
        return NULL;
    }
    Block* ptr = my_malloc_rec(size + sizeof(Block));
    if(ptr){
        return (void*)(ptr + 1);
    }
    return NULL;
}

static void my_free_rec(void* ptr){
    uint64_t size = ((Block*)ptr)->size;
    int i = get_index(size);

    uint64_t adrress_diff_from_base = ptr - start_heap;
    Block* buddy = (Block *) (get_buddy(adrress_diff_from_base, i) + (uint64_t ) start_heap);
    Block** current = &free_lists[i];

    while (*current != NULL && *current != buddy) {
        current = &((*current)->next);
    }

    if(*current != buddy){
        ((Block *)ptr)->next = free_lists[i];
        free_lists[i] = ptr;
        mem_status.reserved -= block_size(i);
        mem_status.free += block_size(i);
    }else{
        *current = buddy->next;

        if(ptr > (void *)buddy){
            buddy->size = block_size(i + 1);
            my_free_rec(buddy);
        }else{
            ((Block *)ptr)->size = block_size(i + 1);
            my_free_rec(ptr);
        }
    }
}

void my_free(void *ptr){
    my_free_rec(((Block*)ptr) - 1);
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








