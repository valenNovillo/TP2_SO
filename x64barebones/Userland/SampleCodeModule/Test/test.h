#ifndef TEST_MM_H
#define TEST_MM_H

#include <stdint.h>

#define MAX_PROCESSES "128"

uint64_t test_mm(uint64_t argc, char *argv[]);
int64_t test_processes(uint64_t argc, char *argv[]);

#endif