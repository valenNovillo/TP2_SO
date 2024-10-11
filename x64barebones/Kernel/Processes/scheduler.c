#include "../include/scheduler.h"
#include "../include/processes.h"
#include "../include/linkedList.h"

typedef struct SchedulerCDT {
    Node* processes[MAX_PROCESSES];
    LinkedList ready_processes;
    uint16_t running_pid;
    uint16_t next_pid;
    uint16_t process_count;
    uint16_t foreground_pid;
    int8_t pending_rounds;
} SchedulerCDT;

Scheduler scheduler;
char is_creating = 0;

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

set_pid_on_array(uint16_t pid, Node * process_node){
    scheduler->processes[pid] = process_node;
    scheduler->process_count++;
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

int32_t setPriority(uint16_t pid, uint8_t new_prio) {
    Node* node = scheduler.processes[pid];
    if (node == NULL || pid == TRIVIAL_PID || new_prio >= QTY_PRIORITIES) {
        return -1;
    }
    PCB* pcb = (PCB*)node->data;
    pcb->priority = new_prio;
    return new_prio;
}

void* schedule(void* last_rsp){
    if (is_creating) {
        return last_rsp;
    }

    static int first_round = 1;
    Node* running = scheduler->processes[scheduler->running_pid];
    if (scheduler->pending_rounds > 0 && running != NULL) {
        scheduler->pending_rounds--;
        return last_rsp;
    }
    if (running != NULL && ((PCB*)running->data)->p_state == RUNNING && scheduler->running_pid != DEFAULT_PID) {
        if (((PCB*)running->data)->priority > 0) {
            ((PCB*)running->data)->priority--;
        }
        ((PCB*)running->data)->p_state = READY;
        remove(scheduler->ready_processes, running);
        queue(scheduler->ready_processes, running);
    }
    else if (running != NULL && scheduler->running_pid == DEFAULT_PID) {
        ((PCB*)running->data)->p_state = READY;
    }

    uint16_t new_PID = getNextReadyProcess();
    PCB* new_P = scheduler->processes[new_PID]->data;
    scheduler->pending_rounds = new_P->priority + 1;
    if (!first_round && running != NULL) {
        ((PCB*)running->data)->rsp = last_rsp;
    }
    else {
        first_round = 0;
    }

    new_P->p_state = RUNNING;
    scheduler->running_pid = new_PID;
    return new_P->rsp;

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

LinkedList get_ready_list() {
    return scheduler->ready_processes;
}

void set_creating(uint8_t creating) {
    is_creating = creating;
}