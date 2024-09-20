#ifndef AUDIODRIVER_H
#define AUDIODRIVER_H

#include <stdint.h>

extern void sound(uint32_t f);
extern void noSound();

//función para producir beeps de sonido por una duración de tiempo "duration" y con una frecuencia "frequency"
void beep(uint32_t frequency, uint64_t duration);


#endif //AUDIODRIVER_H