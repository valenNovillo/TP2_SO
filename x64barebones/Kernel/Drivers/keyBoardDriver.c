#include "include/keyBoardDriver.h"

#include <stdint.h>
#include "include/videoDriver.h"

//=========================================================================FUNCIONES AUXILIARES==================================================================================================

/*Función que determina si se debe usar el conjunto de caracteres alternativo (con Shift). Si bloqMayusc está 
habilitado y es una letra, se utiliza el conjunto alternativo. Si Shift está habilitado, se invierte el valor de alternate.*/
static char checkAlternative(unsigned char code);

/*Función que convierte un código de escaneo en su representación de carácter. checkAlternative determina si se debe 
usar el conjunto de caracteres sin Shift o con Shift.*/
static unsigned char getStringFromCode(unsigned char code);

//Borra la ultima entrada del buffer y retrocede una posicion el writeIdx del buffer
static void removeLastFromBuff();

//===============================================================================================================================================================================================

// variable que indica el estado del bloqueo de mayúsculas (Mayusq Lock) en el teclado. Su propósito es rastrear si la tecla Caps Lock está activada o desactivada, lo que afecta cómo se interpretan las pulsaciones de teclas, específicamente las letras.
char bloqMayusq_enabled = 0; 

//Variable que indica si la tecla shift esta prendida o no.
char shift_enabled = 0;

// Variable que indica si es una tecla del taclado extendido
char extended_enabled = 0;

//variable que indica si la tecla ctrl se encuentra habilitada
char ctrl_enabled = 0;

//Define un búfer circular de BUFF_SIZE (64 bytes) para almacenar los códigos de escaneo del teclado 
unsigned int buffer[SIZE_BUFF] = {0};

//índices para leer y escribir en este búfer.
static int readIdx = 0;
static int writeIdx = 0;

//Variable global que determina si se guardan los caracteres en el buffer
static unsigned char reading = 0;

//Define una matriz de códigos de escaneo, donde cada código de escaneo corresponde a un carácter dependiendo de si 
//la tecla Shift está presionada o no.
//el número 84 se elige para asegurarse de que la matriz puede almacenar un mapeo completo para todos los códigos 
//de escaneo que el teclado puede generar, proporcionando un rango suficiente para cubrir todas las teclas en un 
//teclado estándar y permitiendo la gestión de teclas especiales y cualquier posible expansión futura.
unsigned char scan_codes[][94] = {
        {//SIN SHIFT
                NOT_USE, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',
                '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
                CTRL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
                SHIFT, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', SHIFT,
                '*', ALT, ' ', CAPS, NOT_USE, NOT_USE, NOT_USE,
                NOT_USE, NOT_USE, NOT_USE, NOT_USE, NOT_USE, NOT_USE, NOT_USE,
                NOT_USE, NOT_USE, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'
        },
        {//CON SHIFT
                NOT_USE, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',
                '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
                CTRL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',
                SHIFT, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', SHIFT,
                '*', ALT, ' ', CAPS, NOT_USE, NOT_USE, NOT_USE,
                NOT_USE, NOT_USE, NOT_USE, NOT_USE, NOT_USE, NOT_USE, NOT_USE,
                NOT_USE, NOT_USE, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'
        }
};

static char checkAlternative(unsigned char code) {
    char alternative = 0; //Se declara una variable alternate que se utilizará para determinar si se debe usar el conjunto de caracteres alternativos (es decir, los caracteres correspondientes a cuando Shift o mayusqbock están activos).
    char isLetter = (code >= 0x10 && code <= 0x19) || (code >= 0x1E && code <= 0x26) || (code >= 0x2C && code <= 0x32);//0x10 a 0x19: Q a R , 0x1E a 0x26: A a G , 0x2C a 0x32: Z a M //Estas son las posiciones de las teclas de las letras en el teclado estándar.
    alternative = (isLetter && bloqMayusq_enabled);//Si isLetter es verdadero (el código de escaneo es una letra) y bloqMayusq_enabled está activado, alternate se establece en 1. Esto significa que si el Bloq Mayús está activado y se presionó una letra, se usará el conjunto de caracteres alternativos.
    if(shift_enabled) { //Si shift_enabled está activado (es decir, una tecla Shift está presionada), el valor de alternate se invierte. Esto se debe a que Shift y Caps Lock tienen un efecto contrario en las letras: Si Caps Lock está activado y se presiona Shift, se debe usar el conjunto normal de caracteres. Si Caps Lock no está activado y se presiona Shift, se debe usar el conjunto alternativo de caracteres.
        alternative = !alternative;
    }
    return alternative; //Finalmente, se devuelve el valor de alternate. Este valor (0 o 1) indicará cuál conjunto de caracteres usar al obtener el carácter correspondiente al código de escaneo.
}

static unsigned char getStringFromCode(unsigned char code){
    char alternate = checkAlternative(code);
    return scan_codes[(int)alternate][code];
}

static void removeLastFromBuff() {
    writeIdx = (writeIdx - 1)%SIZE_BUFF;
}


void keyboardHandler(uint64_t infoRegs){
    
    REGISTERS * regsPointer = (REGISTERS *) infoRegs;

    int key = keyPress();
    if (key==ALT_SCAN) {
         // ctrl pressed or realised
    } else if (key == LEFT_CTRL_PRESSED || key == LEFT_CTRL_RELASED) {
        ctrl_enabled =! ctrl_enabled;
    } else if(key== RIGHT_SHIFT_PRESS || key == RIGHT_SHIFT_RELEASE || key == LEFT_SHIFT_PRESS || key == LEFT_SHIFT_RELEASE){
        shift_enabled=!shift_enabled;
    }else if(key==BLOQ_MAYUSQ){
        bloqMayusq_enabled = !bloqMayusq_enabled;
    } else if (ctrl_enabled && key == 0x13) {
        save();
        cleanScreen();
        printRegs(*regsPointer);
        putString(2, "Press ESC to go back to shell\n", 30);
        while(keyPress() != ESC) {
            _hlt();
        }
        load();
        ctrl_enabled =! ctrl_enabled;
        removeLastFromBuff();
    } else if (reading && key <= 0x53) {
        buffer[writeIdx] = getStringFromCode(key);
        writeIdx = (writeIdx+1)%SIZE_BUFF;
    }
}


//Devuelve el siguiente código de escaneo del búfer y avanza el índice de lectura.
int next(){
    unsigned int toReturn = buffer[readIdx];
    readIdx = (readIdx + 1)%SIZE_BUFF; //Se incrementa el índice de lectura readIdx en 1. Sin embargo, si readIdx alcanza BUFF_SIZE (el tamaño del búfer), este se resetea a 0 utilizando el operador módulo %. Esto es necesario para mantener readIdx dentro de los límites del búfer circular.
    return toReturn;
}

//Verifica si hay nuevos códigos de escaneo disponibles en el búfer comparando los índices de lectura y escritura. Si no son iguales, hay datos disponibles para leer.
char hasNext(){
    return readIdx != writeIdx;
}


int fillBuf(char * buf, int count) {
    int i;
    for(i = 0; (i < count) && hasNext(); i++) { 
        buf[i] = next();
    }
    return i;
}


unsigned char setReading(unsigned char flag) {
    if (flag != 0 && flag != 1) 
        return -1;
    reading = flag;
    return reading;
}