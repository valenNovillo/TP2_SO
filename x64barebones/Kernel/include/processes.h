#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdint.h>

#include "typedef_process.h"

char is_waiting(PCB* pcb, int16_t pid_to_wait);
int16_t create_process(Main process_main, char** args, char* name, uint8_t priority, int16_t fds[]);
void free_process_memory(PCB * pcb);
InfoProcess* process_info_load(PCB* pcb);
int16_t * get_fds();
void ps();

#endif