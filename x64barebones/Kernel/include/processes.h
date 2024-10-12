#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdint.h>

#include "typedef_process.h"
#include "../Drivers/include/fileDescriptors.h"

#define MAX_PROCESSES 256 
#define STACK_SIZE 4096
#define BACKGROUND 0
#define FOREGROUND 1
#define DEFAULT_PID 0
#define MAX_PRIORITY 5
#define DOS 2



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
    uint8_t run_mode;// 1 if it's foreground
    unsigned char childrenCount;
    uint16_t children[MAX_PROCESSES];
    int16_t fds[FDS];
    int32_t ret;

} PCB;

typedef struct InfoProcess {
    char *name;
    uint16_t pid;
    uint16_t parent_pid;
    uint8_t priority;
    void* rsp;
    void* rbp;
    uint8_t is_fg;
    PState state;
} InfoProcess;

void is_waiting(PCB* pcb, int16_t pid_to_wait);
int16_t create_process(Main process_main, char** args, uint8_t run_mode, char* name, uint8_t priority, int16_t fds[]);
static void initialize_process(PCB* pcb, Main main_func, char** args, char* name, uint8_t priority, int16_t fds[], uint8_t run_mode);
void free_process_memory(PCB * pcb);
void ps();

#endif