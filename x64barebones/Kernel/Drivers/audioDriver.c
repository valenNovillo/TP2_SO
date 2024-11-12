// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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