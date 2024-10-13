#include "include/timeDriver.h"
#include "../include/scheduler.h"
#include "include/videoDriver.h"

static unsigned long ticks = 0;

void timer_handler(uint64_t infoRegs) {
	ticks++;
    /*REGISTERS* aux = (REGISTERS*) infoRegs;
    aux->rsp = (uint64_t)schedule((void*)infoRegs);*/
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void wait(unsigned int ms){
    int count = ticks_elapsed();

    while((float)ticks_elapsed() < (float)count + (float)(ms *18)/(1000))
        _hlt();

}

