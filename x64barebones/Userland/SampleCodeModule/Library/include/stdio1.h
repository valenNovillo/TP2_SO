#ifndef STDIO1_H
#define STDIO1_H

#include <stdint.h>
#include <stdarg.h>
#include "string.h"
#include "unistd1.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define NO_INPUT -1

//función que lee un caracter por entrada estándar
int getChar();

//función que imprime un caracter por salida estándar
int putChar(int ch);

//función que lee desde entrada estandar y guarda lo leido en una variable que se le pasa por parametro
int scanf1(char* format, ...);

//función que imprime por salida estandar una cadena de caracteres
void printf(char * string, ...);

//función que imprime por salida de errores una cadena de caracteres
void printErr(char * buff);

//función que lee caracteres desde el fd hasta que aparezca un cierto caracter 
int readByDelim(int fd, char * buff, int count, char delim);

//función que convierte un entero n a una cadena de caracteres y la almacena en el buffer s.
void intToString(int n, char* s);

#endif //STDIO_H