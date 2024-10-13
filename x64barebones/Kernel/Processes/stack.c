#include <stdint.h>
#include "../include/stack.h"
#include "../include/processes.h"

void* stack_ptrs[MAX_PROCESSES];
void* start;
int current;

void stack_init(void* ptr) {
    start = ptr;
    current = 0;
    void* aux; 
    for(int i = 0; i < MAX_PROCESSES; i++) {
        aux = start + STACK_SIZE*(i+1);
        stack_ptrs[i] = aux;
    }
}

void* create_stack() {
    if (current < MAX_PROCESSES) {
        return stack_ptrs[current++];
    } else {
        return NULL;
    }
}

void free_stack(void * ptr) {
    stack_ptrs[--current] = ptr;
}

int get_pid_from_stack(void* stack_ptr) {
    return (int)(stack_ptr - start) / STACK_SIZE - 1;
}