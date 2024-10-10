#include <stdint.h>

#include "../include/lib.h"
#include "../include/memoryManager.h"
#include "../include/processes.h"

void* stack_ptrs[MAX_PROCESSES];
void* start;
int current;

void stack_init(void* ptr) {
    start = ptr;
    current = 0;
    for(int i = 0; i < MAX_PROCESSES; i++) {
        stack_ptrs[i] = ptr + STACK_SIZE*i;
    }
}

void* create_stack() {
    if (current < BLOCK_COUNT) {
        return stack_ptrs[current++];
    } else {
        return NULL;
    }
}

void free_stack(void * ptr) {
    free_ptrs[--current] = ptr;
}

PCB * create_process(Main process_main, char **args, uint8_t run_mode, char *name, uint8_t priority, int16_t fds[], uint16_t parent_pid) {
    PCB *process_pcb;

    if(get_process_count() == MAX_PROCESSES ||process_main == NULL || fds == NULL || name == NULL 
        || priority > MAX_PRIORITY || run_mode > FOREGROUND) {
            
        return NULL;
    }

    if((process_pcb = my_malloc(sizeof(PCB))) == NULL)
        return NULL;

    initialize_process(process_pcb, process_main, args, name, priority, fds, run_mode); 
        
    //TO-DO: LOGICA PARA METERLO AL SCHEDULER
    
}

void initialize_process(PCB *pcb, Main main_func, char **args, char *name, uint8_t priority, int16_t fds[], uint8_t run_mode){
    pcb->pid = get_stack_pid(pcb->rsb);

    if (pcb->pid != SHELL_PID && fds[0] == STDIN) {
        PCB * parent_PCB = get_running_process(); //TO-DO
        pcb->parent_pid = parent_PCB->pid;
        if(parent_PCB->run_mode == FOREGROUND) {
            parent_PCB->fds[STDIN] = CTE; //TO-DO: REVISE
        }
    }     
    
    pcb->priority = priority;

    int name_len = strLen(name) + 1;
    pcb->name = my_malloc(name_len + 1);
    memcpy(pcb->name, name, name_len + 1);
    pcb->name[name_len - 1] = 0;
    pcb->waiting_pid = 0;
    pcb->p_state = READY;
    pcb->ret = NULL;

    for(int i = 0; i<MAX_PROCESSES; i==){
        pcb->children[i] = 0;
    }
    pcb->childrenCount = 0;
    
    for(int i = 0; i < FDS; i++) {
        pcb->fds[i] = fds[i];
    }

    if(fds[STDIN] == STDIN)
    {
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

    if(process_pcb->rsb = create_stack() == NULL) {
        return NULL;
    }
    
    process_pcb->rsp = initialize_stack(process_pcb->rsb); //TO-DO

}

void freeProcessMemory(PCB * pcb){
    my_free(pcb->name);
    my_free(pcb->argv);
    my_free(pcb);
}

static int get_stack_pid(void * stack_ptr) {
    return (int)(stack_ptr - start) / STACK_SIZE;
}


