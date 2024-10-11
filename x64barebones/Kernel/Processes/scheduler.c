#include "../include/scheduler.h"
#include "../include/processes.h"
#include "../include/linkedList.h"

typedef struct SchedulerCDT{
    Node* processes[MAX_PROCESSES];
    LinkedList ready_processes;
    uint16_t running_pid;
    uint16_t next_pid;
    uint16_t process_count;
    uint16_t foreground_pid;
    int8_t pending_rounds;
} SchedulerCDT;

Scheduler scheduler;
char is_creatitng = 0;

void initializeScheduler() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        scheduler->processes[i] = NULL;
    }
    scheduler->ready_processes = createLinkedList();
    scheduler->next_pid = 0;
    scheduler->foreground_pid = 0;
    scheduler->pending_rounds = 0;
    scheduler->running_pid = 0;
}

static uint16_t getNextReadyProcess() {
    startIterator(scheduler->ready_processes);
    Node* node = ((Node*)listNext(scheduler->ready_processes));
    if (node == NULL) {
        return DEFAULT_PID;
    }
    return ((PCB*)node->data)->pid;
}

PState setState(uint16_t pid, PState new_state) {
    Node* node = scheduler->processes[pid];
    if (node == NULL || pid == DEFAULT_PID || new_state == RUNNING) {
        return -1;
    }
    PCB* pcb = (PCB*)node->data;
    if (new_state == pcb->p_state) {
        return new_state;
    }
    pcb->p_state = new_state;
    if (new_state == BLOCKED) {
        remove(scheduler->ready_processes, node);
        if (pid == scheduler->running_pid) {
            yieldNoSti();
        }
    }
    else if (new_state == READY) {
        push(scheduler->ready_processes, node);
    }
    return new_state;
}

void* schedule(void* last_rsp){
    if (is_creatitng) {
        return last_rsp;
    }

    

    
}

void set_foreground(uint16_t pid) {
    scheduler->foreground_pid = pid;
}

PCB* get_running_process() {
    return scheduler->running_pid;
}

int get_processes_count() {
    return scheduler->process_count;
}

void set_creating(uint8_t creating) {
    is_creatitng = creating;
}