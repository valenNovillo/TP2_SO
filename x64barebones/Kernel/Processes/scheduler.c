#include "../include/scheduler.h"
#include "../include/processes.h"
#include "../include/linkedList.h"
#include "../Drivers/include/timeDriver.h"
#include "../include/memoryManager.h"

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

void initialize_scheduler() {
    scheduler = my_malloc(sizeof(SchedulerCDT));
    for (int i = 0; i < MAX_PROCESSES; i++) {
        scheduler->processes[i] = NULL;
    }
    scheduler->ready_processes = createLinkedList();
    scheduler->next_pid = 0;
    scheduler->foreground_pid = 0;
    scheduler->pending_rounds = 0;
    scheduler->running_pid = 0;
    scheduler->process_count = 0;
}

static uint16_t get_next_ready_process() {
    startIterator(scheduler->ready_processes);
    Node* node = ((Node*)listNext(scheduler->ready_processes));
    if (node == NULL) {
        return DEFAULT_PID;
    }
    return ((PCB*)node->data)->pid;
}

void set_pid_on_array(uint16_t pid, Node * process_node){
    scheduler->processes[pid] = process_node;
    scheduler->process_count++;
}

PState set_state(uint16_t pid, PState new_state) {
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
            yield();
        }
    }
    else if (new_state == READY) {
        push(scheduler->ready_processes, node);
    }
    return new_state;
}

PState get_state(int16_t pid)
{
    Node* node = scheduler->processes[pid];
    if(node == NULL){
        return -1;
    }

    PCB* pcb = (PCB*)node->data;
    return pcb->p_state;
}

int32_t set_priority(uint16_t pid, uint8_t new_p) {
    Node* node = scheduler->processes[pid];
    if (node == NULL || pid == DEFAULT_PID || pid >= MAX_PROCESSES || new_p < MIN_PRIORITY || new_p > MAX_PRIORITY) {
        return -1;
    }
    PCB* pcb = (PCB*)node->data;
    pcb->priority = new_p;
    return new_p;
}

void* schedule(void* last_rsp){
    if (is_creating) {
        return last_rsp;
    }
   
    static int first_round = 1;
    Node* running = scheduler->processes[scheduler->running_pid];
    PCB * running_pcb = ((PCB*)running->data);
    if (scheduler->pending_rounds > 0 && running != NULL) {
        scheduler->pending_rounds--;
        return last_rsp;
    }

    
    if (running != NULL && running_pcb->p_state == RUNNING && scheduler->running_pid != DEFAULT_PID) {
        /*if (((PCB*)running->data)->priority > 0) {
            ((PCB*)running->data)->priority--; //TO-DO revise
        }*/
        ((PCB*)running->data)->p_state = READY;
        remove(scheduler->ready_processes, running);
        queue(scheduler->ready_processes, running);
    }
    else if (running != NULL && scheduler->running_pid == DEFAULT_PID) {
        ((PCB*)running->data)->p_state = READY;
    }

    uint16_t new_PID = get_next_ready_process();
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

uint16_t get_running_process_pid() {
    return scheduler->running_pid;
}

PCB* get_running_process() {
    return (PCB*)(scheduler->processes[scheduler->running_pid]->data);
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

uint64_t wait_pid(int16_t pid) {
    if (pid != DEFAULT_PID && pid != SHELL_PID && scheduler->processes[pid] != NULL) {
        ((PCB*)scheduler->processes[scheduler->running_pid]->data)->waiting_pid = pid;
        set_state(scheduler->running_pid, BLOCKED);
        return ((PCB*)scheduler->processes[scheduler->running_pid]->data)->ret;
    }
    return 0;
}

uint16_t get_pid() {
    return scheduler->running_pid;
}

InfoProcess** processes_info() {
    int processes_count = get_processes_count();
    
    InfoProcess** to_return = my_malloc(sizeof(InfoProcess*) * processes_count);
    if(to_return == NULL)
        return NULL;

    Node* current;
    for (int i = 0, idx = 0; i < MAX_PROCESSES && idx < processes_count ; i++) {
        current = scheduler->processes[i];
        if (current != NULL) {
            to_return[idx] = process_info_load((PCB*)(current->data));
            idx++;
        }
    }
    return to_return;
}

void yield(){
    scheduler->pending_rounds = 0;
    call_timer_tick();
}

int32_t kill_process(uint16_t pid, int32_t ret){
    if(pid <= UNO || pid >= MAX_PROCESSES){ //if pid = 0 or pid = 1 it's default or shell
        return -1;
    }

    Node* process_to_kill = scheduler->processes[pid];

    if(process_to_kill == NULL){
        return -1;
    }

    PCB* pcb_to_kill = (PCB*)(process_to_kill->data);

    if(pcb_to_kill->p_state != BLOCKED){
        remove(scheduler->ready_processes, process_to_kill);
    }

    pcb_to_kill->ret = ret;
    pcb_to_kill->p_state = TERMINATED;

    Node* parent_process = scheduler->processes[pcb_to_kill->parent_pid];
    if(parent_process != NULL){
        PCB* parent_process_pcb = (PCB*)parent_process->data;
        if(is_waiting(parent_process_pcb, pcb_to_kill->pid)){
            set_state(pcb_to_kill->parent_pid, READY);
            ((PCB*)parent_process->data)->ret = ret;
        }
        if(pcb_to_kill->run_mode){
            scheduler->foreground_pid = parent_process_pcb->pid;
            parent_process_pcb->run_mode = 1;
        }
    }

    //Adoption!
    for(int i = 0; i < pcb_to_kill->childrenCount; i++){
        uint16_t child_pid = pcb_to_kill->children[i];
        if(scheduler->processes[child_pid] != NULL){
            ((PCB*)scheduler->processes[child_pid]->data)->parent_pid = pcb_to_kill->parent_pid;
            unsigned char childrenCount = ((PCB*)scheduler->processes[pcb_to_kill->parent_pid]->data)->childrenCount;
            ((PCB*)scheduler->processes[pcb_to_kill->parent_pid]->data)->children[childrenCount] = child_pid;
            ((PCB*)scheduler->processes[pcb_to_kill->parent_pid]->data)->childrenCount++;
        }
    }

    free_process_memory(pcb_to_kill);
    //mem_free(process_to_kill);//Para qué?? ¿Qué mas tenemos que liberar?
    scheduler->process_count--;
    my_free(scheduler->processes[pid]); //TO-DO: revise
    scheduler->processes[pid] = NULL;
    
    if(pid == scheduler->running_pid){
        yield();
    }

    return 0;
}


int block_process(uint16_t pid) {
   if(set_state(pid, BLOCKED) != BLOCKED) {
        return -1;  
    }
    return 0;
}

int unblock_process(uint16_t pid) {
    if(set_state(pid, READY) != READY) {
        return -1;
    }    
    return 0;
}

void kill_FG(){
    if(scheduler->foreground_pid > 2){
        kill_process(scheduler->foreground_pid, -1);
    }
}
