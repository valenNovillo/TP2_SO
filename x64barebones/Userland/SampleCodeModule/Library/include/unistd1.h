#ifndef UNISTD1_H
#define UNISTD1_H

#include "../../../include/types.h"
#include <stdint.h>
#include <sys/types.h>

//todas estas funciones son llamadas a sycalls

typedef int (*Main)(int argc, char **args);

typedef void* sem_ptr; 

int read(int fd, char * buff, int count);
ssize_t write(int fildes, const void *buf, size_t nbyte);
void beep(uint64_t freq , uint64_t duration);
int getSecondsElapsed();
void sizeInc();
void sizeDec();
int getSeconds();
int getMinutes();
int getHours();
void cleanScreen();
void sleep(unsigned int ms);
void setColor(unsigned char b, unsigned char g, unsigned char r);
void setSize(unsigned int size);
void printSquare(unsigned char color[3], int x, int y, int length);
int reading(unsigned char flag);
int16_t create_process(Main process_main, char** args, char* name, uint8_t priority, int16_t fds[]);
uint16_t get_pid();
int32_t kill_process(uint16_t pid);
int32_t set_priority(uint16_t pid, uint8_t new_p);
int block_process(uint16_t pid);
int unblock_process(uint16_t pid);
void yield();
uint64_t wait_pid(int16_t pid);
int ps(int16_t fds[]);
void _hlt();
sem_ptr my_sem_create(uint64_t id, uint64_t value);
void my_sem_destroy(sem_ptr ptr);
sem_ptr my_sem_open(uint8_t id);
uint8_t my_sem_post(sem_ptr ptr);
uint8_t my_sem_wait(sem_ptr ptr);
int print_mem_status(int16_t fds[]);
void* my_malloc(uint64_t size);
void my_free(void * ptr);
uint16_t get_state(uint16_t pid);
int16_t open_pipe_for_pid(int16_t id, int16_t pid, char mode);
int16_t open_pipe(int16_t id, char mode);
void close_pipe_for_pid(int16_t id, int16_t pid);
int16_t* get_fds();
uint8_t transer_blocked_process_by_sem(sem_ptr from, sem_ptr to);

#endif //UNISTD1_H