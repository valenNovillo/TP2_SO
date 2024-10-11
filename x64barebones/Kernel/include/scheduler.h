#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h>

#include "linkedList.h"

typedef struct SchedulerCDT* Scheduler;

int32_t setPriority(uint16_t pid, uint8_t new_prio);
void initializeScheduler();
void set_foreground(uint16_t pid);
PCB* get_running_process();
int get_processes_count();
LinkedList get_ready_list();
set_pid_on_array(uint16_t pid, Node * process_node);

#endif //SCHEDULER_H