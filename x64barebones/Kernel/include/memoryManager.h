#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stddef.h>
#include <stdint.h>

#define BLOCK_COUNT 8192
#define BLOCK_SIZE 128
#define MEMORY_SIZE "1048576"

void my_mm_init(void* ptr);
void* my_malloc(uint64_t size);
void my_free(void * ptr);

#endif