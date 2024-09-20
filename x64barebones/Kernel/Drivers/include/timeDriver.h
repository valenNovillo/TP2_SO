#ifndef _TIMEDRIVER_H_
#define _TIMEDRIVER_H_

extern int bringSeconds();
extern int bringMinutes();
extern int bringHours();

void timer_handler();

int ticks_elapsed();

int seconds_elapsed();

//espera una cantidad ms de milisegundos
void wait(unsigned int ms);

#endif
