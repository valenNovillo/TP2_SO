#ifndef FILEDESCRIPTOR_H
#define FILEDESCRIPTOR_H

#include "stdint.h"
#define DOS 2
#define EOF -1

int16_t open_pipe(int16_t id, char mode);
int16_t open_pipe_for_pid(int16_t id, int16_t pid, char mode);
void close_pipe_for_pid(int16_t id, int16_t pid);
int write_on_file(int16_t fd, char* buff, unsigned long len);
int read_on_file(int16_t fd,char* target, unsigned long len);
void set_pipe(char value);
char is_pipe_func();

#endif //FILEDESCRIPTOR_H
