#ifndef SHELL_H
#define SHELL_H

#include "../../Library/include/stdio1.h"
#include "../../Library/include/unistd1.h"
#include "../../Library/include/string.h"
#include "../../Library/include/time.h"
#include "music.h"

#define BUFF_SIZE 100

extern void test_io_exception();

//===================================================================================== PROTOTIPOS ===================================================================================================================

//función que ejecuta todo lo correspondiente a la terminal
void shell();

//función que implementa el comando "help" para imprimir un menu de ayuda con los distintos programas disponibles en pantalla
void help();

//función que implementa el comando "inc" para agrandar el tamaño del texto en pantalla
void zoomIn();

//función que implementa el comando "dec" para reducir el tamaño del texto en pantalla
void zoomOut();

//función que implementa el comando "time" que imprime la hora local en pantalla
void time();

//función que implementa el comando "clean" que limpia el contenido de la pantalla
void clean();

//función que implementa el comando "ioexception" que genera una excepción por código de operación inválido
void ioexception();

//función que implementa el comando "zeroexception" que genera una excepción por división por cero
void zeroexception();

//función que implementa el comando "eliminator" que inicia una nueva partida del juego eliminator
void playEliminator();

//función que implementa el comando "playsong" que despliega una lista con canciones para reproducir
void playSong();

//funcion para vincular cada comando ingresado desde la shell con su implementacion
void findCommand(char * buffer);

//funcion para resetear el contenido del buffer de la shell
void resetBuffer();

void test_process();

void ps_commmand();

void test_priority();

//========================================================================================================================================================================================================================


#endif //SHELL_H
