#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <stdint.h>
#include "../Drivers/include/fileDescriptors.h"

#define STATES 5
#define MAX_PROCESSES 256 
#define BACKGROUND 0
#define FOREGROUND 1
#define DEFAULT_PID 0
#define MAX_PRIORITY 5
#define DOS 2

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


#endif //TYPEDEF_H