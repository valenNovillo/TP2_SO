#include <stdint.h>
#include "include/processes.h"
#include "include/memoryManager.h"

void* stack_ptrs[MAX_PROCESSES];
void* start;
int current;

void stack_init(void* ptr) {
    start = ptr;
    current = 0;
    for(int i = 0; i < MAX_PROCESSES; i++) {
        stack_ptrs[i] = ptr + STACK_SIZE*i;
    }
}

void* create_stack() {
    if (current < BLOCK_COUNT) {
        return stack_ptrs[current++];
    } else {
        return NULL;
    }
}

void free_stack(void * ptr) {
    free_ptrs[--current] = ptr;
}

PCB * create_process(int (*proccess_main) (int argc, char **argv), char **argv, uint8_t run_mode) {
    PCB *process_pcb;
    if((process_pcb = my_malloc(sizeof(PCB))) == NULL)
        return NULL;

    process_pcb->rsb = create_stack();
    

    
}




