#ifndef TIME_H
#define TIME_H

#include "unistd1.h"
#include "stdio1.h"

//recibe un buffer y lo completa con la hora actual. Para que la función funcione correctamente el buffer debe ser de por lo menos de una longitud de 9
void getTime(char * buff);

#endif