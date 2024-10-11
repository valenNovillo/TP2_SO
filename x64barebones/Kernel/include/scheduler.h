#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h>

#include "linkedList.h"
#include "processes.h" 

typedef struct SchedulerCDT* Scheduler;

void initializeScheduler();
void set_foreground(uint16_t pid);
PCB* get_running_process();
int get_processes_count();


#endif //SCHEDULER_H