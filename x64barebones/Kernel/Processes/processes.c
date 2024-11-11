#include <stdint.h>
#include "../include/lib.h"
#include "../include/memoryManager.h"
#include "../include/processes.h"
#include "../include/stack.h"
#include "../include/scheduler.h"
#include "../Drivers/include/videoDriver.h"

static void main_wraper(int argc, char** argv, Main main ){
    int32_t return_value = main(argc, argv);
    kill_process(get_running_process()->pid, return_value);
}

static int initialize_process(PCB* pcb, Main main_func, char** args, char* name, uint8_t priority, int16_t fds[]){

    if((pcb->rbp = create_stack()) == NULL){
        return -1;
    }

    pcb->pid = get_pid_from_stack(pcb->rbp);

    if (pcb->pid != DEFAULT_PID){
        PCB* parent_PCB = get_running_process(); 
        pcb->parent_pid = parent_PCB->pid;
        parent_PCB->children[parent_PCB->childrenCount] = pcb->pid;
        parent_PCB->childrenCount++;

        if (fds[STDIN] == STDIN){
            if(parent_PCB->run_mode == FOREGROUND){
                parent_PCB->fds[STDIN] = NO_INPUT;
                parent_PCB->run_mode = BACKGROUND;
                set_foreground(pcb->pid);
                parent_PCB->waiting_pid = pcb->pid;
                parent_PCB->p_state = BLOCKED;
                remove(get_ready_list(), get_process(parent_PCB->pid));
                pcb->run_mode = FOREGROUND;
                //yield_no_timer_tick();
            } else{
                set_creating(0);
                return -1;
            }
        } else{
            pcb->run_mode = BACKGROUND;
        }
    } else{
        pcb->run_mode = FOREGROUND;
    }
    
    pcb->priority = priority;

    int name_len = strlen(name);
    if ((pcb->name = my_malloc(name_len + 1)) == NULL){
        return -1;
    }
    memcpy(pcb->name, name, name_len);
    pcb->name[name_len] = '\0';

    pcb->waiting_pid = -1;
    pcb->p_state = READY;
    pcb->ret = 0;

    for(int i = 0; i<MAX_PROCESSES; i++){
        pcb->children[i] = 0;
    }
    pcb->childrenCount = 0;
    
    for(int i = 0; i < FDS; i++){
        pcb->fds[i] = fds[i];
    }
        
    //Arguments configuration
    pcb->argc = count_args((void*)args);
    int args_len[pcb->argc];
    for (int i = 0; i < pcb->argc; i++){
        args_len[i] = strlen(args[i]);
    }
    
    int size = sizeof(char**) * (pcb->argc + 1);
    char** args_array = (char**)my_malloc(size);
    
    for (int i = 0; i < pcb->argc; i++){
        args_array[i] = my_malloc(args_len[i]);
        memcpy(args_array[i], args[i], args_len[i]);
    }

    pcb->argv = args_array;
    pcb->argv[pcb->argc] = NULL;
    pcb->rsp = initialize_stack(pcb->rbp, pcb->argc, pcb->argv, main_func, (Main_wrapper)main_wraper); 
    return 0;
}

int16_t create_process(Main process_main, char** args, char* name, uint8_t priority, int16_t fds[]){
    PCB *process_pcb;
    set_creating(1);

    if(get_processes_count() == MAX_PROCESSES || process_main == NULL || fds == NULL || name == NULL 
        || priority > MAX_PRIORITY){
        set_creating(0);
        return -1;
    }

    if((process_pcb = my_malloc(sizeof(PCB))) == NULL){
        return -1;
        set_creating(0);
    }

    if (initialize_process(process_pcb, process_main, args, name, priority, fds) == -1){
        set_creating(0);
        return -1;
    } 
        
    Node* process_node = my_malloc(sizeof(Node));
    if (process_node == NULL){
        set_creating(0);
        return -1;
    }
    process_node->data = (void*) process_pcb;

    if(process_pcb->pid != DEFAULT_PID){
        process_pcb->p_state = READY;
        queue(get_ready_list(), process_node);
    }
    
    set_pid_on_array(process_pcb->pid, process_node);
    set_creating(0);
    if(process_pcb->run_mode == FOREGROUND){
        yield();
    }
    return process_pcb->pid; 
}

void free_process_memory(PCB * pcb){
    my_free(pcb->name);
    for(int i = 0; i < pcb->argc; i++){
        my_free(pcb->argv[i]);
    }
    my_free(pcb->argv);
    free_stack(pcb->rbp);
    my_free(pcb);
}

char is_waiting(PCB* pcb, int16_t pid_to_wait){
    return pcb->waiting_pid == pid_to_wait;
}


InfoProcess* process_info_load(PCB* pcb){
    InfoProcess* aux = my_malloc(sizeof(InfoProcess));
    if(aux == NULL)
        return NULL;
    
    aux->name = pcb->name;
    aux->pid = pcb->pid;
    aux->parent_pid = pcb->parent_pid;
    aux->priority = pcb->priority;
    aux->rsp = pcb->rsp;
    aux->rbp = pcb->rbp;
    aux->is_fg = pcb->run_mode;
    aux->state = pcb->p_state;
    
    return aux;
}

int16_t * get_fds(){
    return get_running_process()->fds;
}

void ps(int16_t fds[]){
    char* foreground[2] = {"Background", "foreground"};
    char* state[STATES] = {"RUNNING", "BLOCKED", "READY", "ZOMBIE", "TERMINATED"};

    InfoProcess** info_processes = processes_info();
    write(fds[STDOUT],"\n", 1);
    
    int process_count = get_processes_count();
    for(int i = 0; i < process_count; i++) {
        write(fds[STDOUT],"Process: ", 9);
        write(fds[STDOUT],info_processes[i]->name, strlen(info_processes[i]->name));

        write(fds[STDOUT], "\tPID: ", 6);
        char string_pid[4];
        int len = intToString(info_processes[i]->pid, string_pid);
        write(fds[STDOUT], string_pid, len);

        write(fds[STDOUT], "\tPriority: ", 11);
        char string_prioridad[1];
        len = intToString(info_processes[i]->priority, string_prioridad);
        write(fds[STDOUT], string_prioridad, len);

        write(fds[STDOUT], "\tRSP: ", 6);
        char string_rsp[8];
        len = decimalToHexadecimal((uint64_t)(info_processes[i]->rsp), string_rsp, 8);
        write(fds[STDOUT], string_rsp, len);

        write(fds[STDOUT], "\tRBP: ", 6);
        char string_rbp[8];
        len = decimalToHexadecimal((uint64_t)(info_processes[i]->rbp), string_rbp, 8);
        write(fds[STDOUT], string_rbp, len);

        write(fds[STDOUT], "\tRunning in: ", 13);
        write(fds[STDOUT], foreground[info_processes[i]->is_fg], strlen(foreground[info_processes[i]->is_fg]));

        write(fds[STDOUT], "\tState: ", 8);
        write(fds[STDOUT], state[info_processes[i]->state], strlen(state[info_processes[i]->state]));

        write(fds[STDOUT], "\n", 1);

        my_free(info_processes[i]);
    }
    
    
    my_free(info_processes);
}


