#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H


#include <stdint.h>

#define INITIAL_SIZE 2
#define UPDATE_SIZE 1
#define HEIGHT_PIXELS 768
#define WIDTH_PIXELS 1024
#define BUFFERSIZE 64


typedef struct Color {
    uint8_t blue;
    uint8_t green;
    uint8_t red; 
} Color;

typedef struct REGISTERS
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;   
    uint64_t rsp;
}REGISTERS;

// ============================================PROTOTIPO DE FUNCIONES===================================================================

/* función que "mueve" (reimprime) todas las lineas en pantalla una posicion mas "arriba" tantas veces como largo tenga la pantalla. De esta forma la 
   pantalla queda totalmente despejada.
*/
void cleanScreen(); 

// función que imprime en forma ordenada el contenido de los resgistros del procesador
void printRegs(REGISTERS r);

// función que hace un backup del estado de la pantalla
void save();

//función que carga el backup de lo que era el estado de la pantalla
void load();

// función que imprime en el fd correspondiente una cadena de caracteres de determinada logitud
int putString(uint64_t fd, char * string, int length);

// función que aumenta en UPDATE_SIZE el tamaño actual de lo que se imprime en pantalla 
void sizeInc();

// función que decrementa en UPDATE_SIZE el tamaño actual de lo que se imprime en pantalla 
void sizeDec();

// función que determina el color de lo que se imprime en pantalla segun el color que le pasan como parametro
void setColor(Color c);

// función que determina el tamaño de lo que se imprime en pantalla segun el tamaño que le pasen por parametro
void setSize(int newSize);

// función que imprime un cuadrado en pantalla, del color, el tamaño y a partir de la posicion (x,y) que le pasan por parametro
void printSquare(Color hexColor, int x, int y, int length);

// =====================================================================================================================================

#endif //VIDEO_DRIVER_H