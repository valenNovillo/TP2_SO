#ifndef UNISTD1_H
#define UNISTD1_H

#include "../../../include/types.h"
#include <stdint.h>
#include <sys/types.h>

//todas estas funciones son llamadas a sycalls

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

#endif //UNISTD1_H