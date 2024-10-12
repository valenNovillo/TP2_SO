#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdint.h>

#include "typedef_process.h"

char is_waiting(PCB* pcb, int16_t pid_to_wait);
int16_t create_process(Main process_main, char** args, uint8_t run_mode, char* name, uint8_t priority, int16_t fds[]);
void free_process_memory(PCB * pcb);
InfoProcess* process_info_load(PCB* pcb);
void ps();

#endif