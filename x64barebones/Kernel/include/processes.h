#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdint.h>

#include "../Drivers/include/fileDescriptors.h"

#define MAX_PROCESSES 256 
#define STACK_SIZE 4096
#define BACKGROUND 0
#define FOREGROUND 1
#define DEFAULT_PID 0
#define MAX_PRIORITY 5

typedef enum PState {RUNNING, BLOCKED, READY, ZOMBIE, TERMINATED} PState;

typedef int (*Main)(int argc, char **args);

typedef struct PCB {
    int16_t waiting_pid; //TODO chequear
    uint16_t parent_pid;
    uint16_t pid;

    void *rsp;
    void *rbp;
    
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

int16_t create_process(int (*proccess_main) (int argc, char **argv), char **argv, uint8_t run_mode);
void initializeProcess(PCB *pcb, uint16_t pid, uint16_t parent_pid, Main main_func, char **args, char *name, uint8_t priority, int16_t fds[]);

#endif