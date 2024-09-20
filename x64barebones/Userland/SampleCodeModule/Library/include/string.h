#include "../../../include/types.h"

#ifndef STRING_H
#define STRING_H

#include <stdint.h>

//función que retorna la longitud de la cadena de caracteres que le pasan como parametro
size_t strlen(const char *cadena);

//función que compara dos cadenas de caracteres. 
// devuelve:
// - 0 si las cadenas son iguales
// - un valor < 0 si la cadena1 es menor que la cadena2 
// - un valor > 0 si la cadena1 es mayor que la cadena2
int strcmp(const char *cadena1, const char *cadena2);

#endif