#ifndef _TIMEDRIVER_H_
#define _TIMEDRIVER_H_

#include <stdint.h>

extern int bringSeconds();
extern int bringMinutes();
extern int bringHours();
extern void _hlt();

void timer_handler(uint64_t infoRegs);

int ticks_elapsed();

int seconds_elapsed();

//espera una cantidad ms de milisegundos
void wait(unsigned int ms);

#endif
