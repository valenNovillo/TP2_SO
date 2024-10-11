#include <stdint.h>
#include "../include/stack.h"
#include "../include/processes.h"

void* stack_ptrs[MAX_PROCESSES];
void* start;
int current;

void stack_init(void* ptr) {
    start = ptr;
    current = 0;
    for(int i = 0; i < MAX_PROCESSES; i++) {
        stack_ptrs[i] = start + STACK_SIZE*(i+1) - 1;
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


void* initialize_stack(void *stack_ptr, int argc, char** argv, Main main_func) {
    StackInit* aux = (StackInit*) stack_ptr;
    aux->ss = 0x0;
    aux->rsp = stack_ptrs;
    aux->rflags = 0x202;
    aux->cs = 0x8;
    aux->rip = main_func;
    aux->rax = 0;
    aux->rbx = 0;
    aux->rcx = 0;
    aux->rdx = 0;
    aux->rbp = stack_ptr;
    aux->rdi = argc;
    aux->rsi = argv;
    aux->r8 = 0;
    aux->r9 = 0;
    aux->r10 = 0;
    aux->r11 = 0;
    aux->r12 = 0;
    aux->r13 = 0;
    aux->r14 = 0;
    aux->r15 = 0;

    return aux - sizeof(aux);
}

int get_pid_from_stack(void* stack_ptr) {
    return (int)(stack_ptr - start) / STACK_SIZE;
}