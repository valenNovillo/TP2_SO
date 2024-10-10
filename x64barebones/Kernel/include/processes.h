#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdint.h>

#include "../Drivers/fileDescriptors.h"

#define MAX_PROCESSES 256 
#define STACK_SIZE 4096
#define BACKGROUND 0
#define FOREGROUND 1
#define SHELL_PID 0

typedef enum PState {RUNNIG, BLOCKED, READY, ZOMBIE, TERMINATED} PState;

typedef int (*Main)(int argc, char **args);

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

void initializeProcess(PCB *pcb, uint16_t pid, uint16_t parent_pid, Main main_func, char **args, char *name, uint8_t priority, int16_t fds[]);

#endif