#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdint.h>

#include "../Drivers/fileDescriptors.h"

#define MAX_PROCESSES 256 
#define STACK_SIZE 4096

typedef enum PState = {RUNNIG, BLOCKED, READY, ZOMBIE, TERMINATED} PState;

typedef struct PCB {
    int16_t waiting_pid;
    uint16_t parent_pid;
    uint16_t pid;

    void *rsp;
    void *rsb;
    
    char *name;
    char **argv;

    uint8_t priority;
    PState p_state;
    uint8_t run_mode;
    unsigned char childrenCount;
    uint16_t children[MAX_PROCESSES];
    int16_t fds[FDS];
    int32_t ret;

} PCB;

void stack_init(void* ptr);
PCB * create_process(int (*proccess_main) (int argc, char **argv), char **argv, uint8_t run_mode);


#endif