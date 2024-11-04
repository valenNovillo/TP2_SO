#ifndef TEST_MM_H
#define TEST_MM_H

#include <stdint.h>

#define MAX_PROCESSES "10"

#ifdef BUDDY_MODE 
 #define MEMORY_SIZE "268435456"
#else
  #define MEMORY_SIZE "16777216"
#endif

uint64_t test_mm(uint64_t argc, char *argv[]);
int64_t test_processes(uint64_t argc, char *argv[]);
uint64_t test_sync(uint64_t argc, char *argv[]);
void test_prio();

#endif