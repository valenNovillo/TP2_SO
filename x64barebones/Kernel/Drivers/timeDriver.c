// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/timeDriver.h"
#include "../include/scheduler.h"
#include "include/videoDriver.h"

static unsigned long ticks = 0;

void timer_handler(uint64_t infoRegs){
	ticks++;
}

int ticks_elapsed(){
	return ticks;
}

int seconds_elapsed(){
	return ticks / 18;
}

void wait(unsigned int ms){
    int count = ticks_elapsed();

    while((float)ticks_elapsed() < (float)count + (float)(ms *18)/(1000))
        _hlt();

}

