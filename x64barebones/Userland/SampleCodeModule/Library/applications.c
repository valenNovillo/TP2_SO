#include "./include/applications.h"
#include "../Library/include/stdio1.h"
#include "../Library/include/unistd1.h"
#include <stddef.h>

#define SLEEP_TIME 5000

typedef enum PState {RUNNING, BLOCKED, READY, ZOMBIE, TERMINATED} PState;


void loop() {
    int pid = get_pid(); 

    while (1) {
        printf("\nHi! I'm process %d and I'll be looping every %d seconds\n", pid, SLEEP_TIME/1000);
        sleep(SLEEP_TIME);  
    }
}

void kill(char *args[]) {
    if (args[0] == NULL) {
        printf("\nPlease specify a process ID to kill.\n");
        return;
    }

    int pid = stringToInt(args[0]); 

    if(kill_process(pid) < 0) {
        printf("\nCould not kill process %d.\n", pid);
        return;
    }

    printf("\nProcess %d has been killed.\n", pid);
}

void nice(char *args[]) {
    if (args[0] == NULL) {
        printf("\nPlease specify the process ID you want to change its priority.\n");
        return;
    }

    if (args[1] == NULL) {
        printf("\nPlease specify the new PRIORITY you want to set.\n");
        return;
    }

    int pid = stringToInt(args[0]); 
    int priority = stringToInt(args[1]); 

    if(set_priority(pid, priority) < 0) {
        printf("\nCould not change process %d priority to %d.\n", pid, priority);
        return;
    }

    printf("\nProcess %d now has priority %d.\n", pid, priority);
}

void block(char *args[]) {
     if (args[0] == NULL) {
        printf("\nPlease specify the process ID you want to change its state\n");
        return;
    }

    int pid = stringToInt(args[0]); 

    PState state = get_state(pid);
    int ret = 0;

    if(state == BLOCKED) {
        ret = unblock_process(pid);
    } else if (state == READY || state == RUNNING) {
        ret = block_process(pid);
    } else {
        printf("\nCould not change process %d state\n", pid);
        return;
    } 

    if(ret == -1) {
        printf("\nCould not change process %d state\n", pid);
        return;
    }
      
    PState new_state = get_state(pid);  

    printf("\nProcess %d changed to state %s \n", pid, new_state == BLOCKED ? "BLOCKED" : (new_state == READY ? "READY" : "RUNNING"));
}