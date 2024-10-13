#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stddef.h>

#include "typedef_process.h"

#define STACK_SIZE 4096

void stack_init(void* ptr);
void* create_stack();
void free_stack(void * ptr);
int get_pid_from_stack(void* stack_ptr);

extern void* initialize_stack(void *stack_ptr, int argc, char** argv, Main main_func);
#endif //STACK_H