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
uint16_t get_foreground_pid();
void set_foreground(uint16_t pid);
uint16_t get_running_process_pid();
PCB* get_running_process();
int get_processes_count();
LinkedList get_ready_list();
void set_pid_on_array(uint16_t pid, Node * process_node);
uint64_t wait_pid(int16_t pid);
uint16_t get_pid();
void* schedule(void* last_rsp);
int block_process(uint16_t pid);
int unblock_process(uint16_t pid);
int32_t kill_process(uint16_t pid, int32_t ret);
void set_creating(uint8_t creating);
InfoProcess** processes_info();
PState get_state(int16_t pid);
int32_t kill_FG();
Node * get_process(uint16_t pid);
char is_waiting_someone(int16_t pid);

#endif //SCHEDULER_H