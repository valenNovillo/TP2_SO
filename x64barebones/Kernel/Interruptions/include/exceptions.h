#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>

extern int keyPress();
extern void _hlt();

//funcion para manejo de excepciones
void exceptionDispatcher(int exception, uint64_t infoRegs);

#endif //EXCEPTIONS_H