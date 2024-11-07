#ifndef FILEDESCRIPTOR_H
#define FILEDESCRIPTOR_H

#include "stdint.h"
#define DOS 2
#define EOF 1

int16_t open_pipe_for_pid(int16_t id, int16_t pid, char mode);
void close_pipe_for_pid(int16_t id, int16_t pid);
int write_on_file(int16_t id, unsigned char *buff, unsigned long len);
int read_on_file(int16_t id, unsigned char *target, unsigned long len);

#endif //FILEDESCRIPTOR_H
