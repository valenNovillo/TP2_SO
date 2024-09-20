#include "include/timeDriver.h"

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
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

