#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h>
#include <stdint.h>
#include "typedef_process.h"
#include "linkedList.h"

typedef struct SchedulerCDT* Scheduler;


extern void call_timer_tick();
PState set_state(uint16_t pid, PState new_state);
void yield();
int32_t set_priority(uint16_t pid, uint8_t new_prio);
void initialize_scheduler();
void set_foreground(uint16_t pid);
uint16_t get_running_process_pid();
int get_processes_count();
LinkedList get_ready_list();
set_pid_on_array(uint16_t pid, Node * process_node);
static uint16_t get_next_ready_process();
uint64_t wait_pid(int16_t pid);
uint16_t get_pid();
void* schedule(void* last_rsp);
int block_process(uint16_t pid);
int unblock_process(uint16_t pid);



#endif //SCHEDULER_H