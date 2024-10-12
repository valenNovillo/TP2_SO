#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h>

#include "linkedList.h"

typedef struct SchedulerCDT* Scheduler;

int32_t set_priority(uint16_t pid, uint8_t new_prio);
void initialize_scheduler();
void set_foreground(uint16_t pid);
PCB* get_running_process();
int get_processes_count();
LinkedList get_ready_list();
set_pid_on_array(uint16_t pid, Node * process_node);
static uint16_t get_next_ready_process();
uint64_t wait_pid(int16_t pid);
uint16_t get_pid();


#endif //SCHEDULER_H