#include <stdint.h>
#include "../include/lib.h"
#include "../include/memoryManager.h"
#include "../include/processes.h"
#include "../include/stack.h"
#include "../include/scheduler.h"


int16_t create_process(Main process_main, char** args, uint8_t run_mode, char* name, uint8_t priority, int16_t fds[]) {
    PCB *process_pcb;
    set_creating(1);

    if(get_processes_count() == MAX_PROCESSES ||process_main == NULL || fds == NULL || name == NULL 
        || priority > MAX_PRIORITY || run_mode > FOREGROUND) {
        set_creating(0);
        return -1;
    }

    if((process_pcb = my_malloc(sizeof(PCB))) == NULL) {
        return -1;
        set_creating(0);
    }

    initialize_process(process_pcb, process_main, args, name, priority, fds, run_mode); 
        
    //TO-DO: LOGICA PARA SCHEDULER
    Node* process_node = my_malloc(sizeof(Node));
    if (process_node == NULL) {
        return -1;
    }
    process_node->data = (void*) process_pcb;

    if(process_pcb->pid != DEFAULT_PID) {
        process_pcb->p_state = READY;
        queue(get_ready_list(), process_node);
    }

    set_creating(0);
    set_pid_on_array(process_pcb->pid, process_node);
    return process_pcb->pid; 
}

static void initialize_process(PCB* pcb, Main main_func, char** args, char* name, uint8_t priority, int16_t fds[], uint8_t run_mode){

    if(pcb->rbp = create_stack() == NULL) {
        return NULL;
    }

    pcb->pid = get_pid_from_stack(pcb->rbp);

    if (pcb->pid != DEFAULT_PID) {
        PCB* parent_PCB = get_running_process(); 
        pcb->parent_pid = parent_PCB->pid;
        parent_PCB->children[parent_PCB->childrenCount] = pcb->pid;
        parent_PCB->childrenCount++;
        if(fds[0] == STDIN && parent_PCB->run_mode == FOREGROUND) {
            parent_PCB->fds[STDIN] = NO_INPUT; //TO-DO: REVISE
            parent_PCB->run_mode = BACKGROUND;
            set_foreground(pcb->pid); 
        } else {
            return -1;
        }
    }     
    
    pcb->priority = priority;

    int name_len = strLen(name) + 1;
    pcb->name = my_malloc(name_len + 1);
    memcpy(pcb->name, name, name_len + 1);
    pcb->name[name_len - 1] = '\0';

    pcb->waiting_pid = -1; //TO-DO: REVISE!!!!
    pcb->p_state = READY;
    pcb->ret = NULL;

    for(int i = 0; i<MAX_PROCESSES; i++) {
        pcb->children[i] = 0;
    }
    pcb->childrenCount = 0;
    
    for(int i = 0; i < FDS; i++) {
        pcb->fds[i] = fds[i];
    }

    if(fds[STDIN] == STDIN) {
        pcb->run_mode = 1; //if process interacts with user stdin then is running in foreground!
    } else {
        pcb->run_mode = 0;
    }
    
    //Arguments configuration
    int argc = count_args((void*)args);
    int total_args_len = 0;
    int args_len[argc];
    for (int i = 0; i < argc; i++) {
        args_len[i] = strLen(args[i]) + 1;
        total_args_len += args_len[i];
    }
    
    int size = sizeof(char**) * (argc + 1);
    char** args_array = (char**)my_malloc(total_args_len + size);
    char* start_ptr = (char*)args_array + size;
    
    for (int i = 0; i < argc; i++) {
        args_array[i] = start_ptr;
        memcpy(start_ptr, args[i], args_len[i]);
        start_ptr += args_len[i];
    }

    pcb->argv = args_array;
    pcb->argv[argc] = NULL;

    pcb->rsp = initialize_stack(pcb->rbp, argc, args, main_func); 
}

void free_process_memory(PCB * pcb){
    my_free(pcb->name);
    my_free(pcb->argv);
    free_stack(pcb->rbp);
    my_free(pcb);
}

