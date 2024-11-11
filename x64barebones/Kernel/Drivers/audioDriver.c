#include "include/audioDriver.h"
#include "../include/lib.h"
#include "include/timeDriver.h"


void beep(uint32_t frequency, uint64_t duration){
    if(frequency != 0){
        sound(frequency);
    }
    wait(duration);
    nosound();
}