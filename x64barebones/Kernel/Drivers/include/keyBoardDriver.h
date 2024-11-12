#ifndef KEYBOARDDRIVER_H
#define KEYBOARDDRIVER_H

#define NOT_USE 0 //Esta constante define un valor (0) para representar teclas que no se utilizan o no son mapeables en el contexto del controlador de teclado.

//Define constantes para los códigos de escaneo de teclas específicas (Shift derecho, Shift izquierdo, su liberación
//, F1, y Bloq Mayús) y el tamaño del búfer del teclado.
#define RIGHT_SHIFT_RELEASE 0xB6
#define LEFT_SHIFT_RELEASE 0xAA
#define LEFT_SHIFT_PRESS 0x2A
#define RIGHT_SHIFT_PRESS 0x36
#define F1 0xAA
#define BLOQ_MAYUSQ 0x3A //La constante BLOQ_MAYUSQ representa el código de escaneo correspondiente a la tecla de Bloq Mayús en un teclado.
#define SIZE_BUFF 64    

//Estas constantes representan los códigos de escaneo de las teclas Alt derecha (0x38) y Control derecha (0x1D). Son usadas para detectar cuándo estas teclas están presionadas.
#define ALT_SCAN 0x38
#define LEFT_CTRL_PRESSED 0x1D
#define LEFT_CTRL_RELASED 0x9D

//Estas constantes definen valores simbólicos para ciertas teclas de control comunes:
#define ESC 1
#define CTRL 2
#define SHIFT 3
#define CAPS 4
#define ALT 5

#include <stdint.h>

//Manejo de interrupciones del teclado que lee un código de tecla presionada, actualiza el estado de las teclas 
//especiales y avanza el índice de escritura en el búfer circular.
void keyboardHandler(uint64_t infoRegs);

//LLena el buffer con count caracteres o hasta que se se vacie el buffer.
//Retorna la cantidade de carcteres cergados en el buffer
int fillBuf(char * buf, int count);

//Funcion usada para cambiar el valor del flgag de reading
unsigned char setReading(unsigned char flag);   

extern void _hlt();
extern int keyPress();


#endif //KEYBOARDDRIVER_H
