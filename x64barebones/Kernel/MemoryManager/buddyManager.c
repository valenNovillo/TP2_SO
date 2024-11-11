#include "../include/memoryManager.h"
#include "../include/lib.h"
#include "../Drivers/include/videoDriver.h"
#include "../Drivers/include/fileDescriptors.h"

#define MIN_BLOCK_SIZE 1024
#define MAX_2_POW 19
#define BLOCKSIZE(i) ((unsigned long long)(1 << (i)) * MIN_BLOCK_SIZE) 
#define GET_BUDDY(b, i) ((((unsigned long long )(b)) ^ (BLOCKSIZE(i))))

typedef struct Block {
    unsigned long long size;
    struct Block *next; 
} Block;

static Block* free_lists[MAX_2_POW +1];
static MemoryStatus mem_status;
static void* start_heap;

static int get_index(uint64_t size){
    int i = 0;
    while (BLOCKSIZE(i) < size) {
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



    if (i > MAX_2_POW) {
        i = MAX_2_POW;
    }

    Block *aligned_start_ptr = (Block *)(((unsigned long long)ptr + 7) & ~7);
    start_heap = aligned_start_ptr;
    free_lists[i] = (Block*)ptr;
    free_lists[i]->next = NULL;
    free_lists[i]->size = BLOCKSIZE(i);
    mem_status.total = free_lists[i]->size;
    mem_status.free = free_lists[i]->size;
    mem_status.reserved = 0;
}

static Block* my_malloc_rec(unsigned long size){
    int i = get_index(size);

     if (i > MAX_2_POW) {
        return NULL;
    }

    if(free_lists[i] != NULL){
        Block* block = free_lists[i];
        free_lists[i] = free_lists[i]->next;
        block->size = BLOCKSIZE(i);
        mem_status.reserved += block->size;
        mem_status.free -= block->size;
        return block;
    }

    Block* block = my_malloc_rec(BLOCKSIZE(i + 1));
    
    if(block != NULL){
        unsigned long long address_diff_from_base = (void *)block - start_heap;
        Block* buddy = (Block*)((GET_BUDDY(address_diff_from_base, i)) + (unsigned long long)start_heap);

        buddy->size = BLOCKSIZE(i);
        buddy->next = free_lists[i];
        free_lists[i] = buddy;

        block->size = BLOCKSIZE(i);
    }

    return block;
}


void * my_malloc(unsigned long size) {
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
    unsigned long size = ((Block*)ptr)->size;
    int i = get_index(size);

    unsigned long long address_diff_from_base = ptr - start_heap;
    Block* buddy = (Block*)((GET_BUDDY(address_diff_from_base, i)) + (unsigned long long)start_heap);
    Block** current = &free_lists[i];

    while (*current != NULL && *current != buddy) {
        current = &((*current)->next);
    }

    if(*current != buddy){
        ((Block *)ptr)->next = free_lists[i];
        free_lists[i] = ptr;
        mem_status.reserved -= BLOCKSIZE(i);
        mem_status.free += BLOCKSIZE(i);
    }else{
        *current = buddy->next;

        if(ptr > (void *)buddy){
            buddy->size = BLOCKSIZE(i + 1);
            my_free_rec(buddy);
        }else{
            ((Block *)ptr)->size = BLOCKSIZE(i + 1);
            my_free_rec(ptr);
        }
    }
}

void my_free(void *ptr){
    my_free_rec(((Block*)ptr) - 1);
}

void print_mem_status(int16_t fds[]) {
    int len;

    write(fds[STDOUT],"\nMemory status:\n", 16);
    
    write(fds[STDOUT],"TOTAL: ", 7);
    char string_total[10];
    len = intToString(mem_status.total, string_total);
    write(fds[STDOUT], string_total, len);

    write(fds[STDOUT], "\nRESERVED: ", 11);
    char string_reserved[10];
    len = intToString(mem_status.reserved, string_reserved);
    write(fds[STDOUT], string_reserved, len);

    write(fds[STDOUT], "\nFREE: ", 7);
    char string_free[10];
    len = intToString(mem_status.free, string_free);
    write(fds[STDOUT], string_free, len);

    write(fds[STDOUT], "\n", 1);
}








