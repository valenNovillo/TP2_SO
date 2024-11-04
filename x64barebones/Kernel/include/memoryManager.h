#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stddef.h>
#include <stdint.h>

typedef struct MemoryStatus {
    uint64_t total;
    uint64_t reserved;
    uint64_t free;
} MemoryStatus;

void my_mm_init(void* ptr);
void* my_malloc(uint64_t size);
void my_free(void * ptr);

extern void _cli(void);
extern void _sti(void);

#endif