#include "include/stdio1.h"

void intToString(int n, char* s);
int readByDelim(int fd, char * buff, int count, char delim);

static int get_char_fd(int16_t fd) {
    char c = 0;
    int count = 0;
    while(count == 0) 
        count = read(fd, &c, 1);
    return count == 1 ? c : -1;
}

static int put_char_fd(int16_t fd, int ch) {
    int n;
    n = write(fd, &ch, 1);
    return n == -1 ? -1 : (unsigned char)ch;
}

int getChar() {
    return get_char_fd(get_fds()[STDIN]);
}

int putChar(int ch) {
    return put_char_fd(get_fds()[STDOUT], ch);
}

int scanf1(char* format, ...) {
    va_list args;
    va_start(args, format);

    int readChars = 0;
    int length;
    char buffer[100];

    int* argD;
    char* sPointer;

    int i = 0;
    while( format[i] != 0 ){
        if( format[i] == '%') {
            i++;

            length = readByDelim(0, buffer, 100, '\n');
            buffer[length] = 0;
            readChars += length;

            putChar(' ');

            switch (format[i]) {
                case 'd':
                    argD = va_arg(args, int*);
                    *argD = 0;
                    for (int j = 0; buffer[j] != 0; ++j)
                        *argD = *argD * 10 + (buffer[j] - '0');
                    break;

                case 's':
                    sPointer = va_arg(args, char*); //obtiene el siguiente argumento de la lista (args) como un puntero a char (sPointer). 
                    for (int j = 0; buffer[j] != 0; ++j)//Copia la cadena buffer a sPointer.
                        sPointer[j] = buffer[j];
                    break;
            }
        }
        i++; //Avanza al siguiente caracter de la cadena de formato

    }

    va_end(args); //Limpia la lista de argumentos variables
    return readChars;//retorna la cantidad de caracteres leídos
}

void fprintf(int16_t fd, char * string, ...) {
    va_list argPointer; 
    va_start(argPointer,string);

    int i=0;
    while(string[i]!=0){ 
        if(string[i] == '%'){ 
            char buffInt[20];
            char * str; 

            switch (string[i+1]) { 
                case 'd':
                    intToString(va_arg(argPointer, int), buffInt);
                    fprintf(fd, buffInt);
                    break;
                case 's':
                    str = va_arg(argPointer, char*);
                    fprintf(fd, str);
                    break;
            }
            i+=2;
        }else if(string[i]=='\\') {
            switch (string[i+1]) {
                case 'n':
                    put_char_fd(fd, '\n'); break;
                case 't':
                    put_char_fd(fd, '\t'); break;
                case '\\':
                    put_char_fd(fd, '\\'); break;

            }
            i+=2;
        }else
            put_char_fd(fd, string[i++]);
    }

    va_end(argPointer);
}


void printf(char * string, ...) {
    va_list args;
    va_start(args, string);
    fprintf(get_fds()[STDOUT], string, args);
    va_end(args);
}


void printErr(char * buff, ...) {
    va_list args;
    va_start(args, buff);
    fprintf(get_fds()[STDERR], buff, args);
    va_end(args);
}


int readByDelim(int fd, char * buff, int count, char delim) {
    if(fd != 0){
        return 0;
    }

    int i = 0; //para contabilizar los caracteres leidos
    char c; //para almacenar el caracter leido


    while((c = getChar()) != delim){ //mientras que el caracter sea distinto del delimitador, seguimos 
        switch (c) {
            case 0: //Si el buffer esta vacío o no se leyó un caracter válido
                break;
            case '\b': //Si c es un carácter de retroceso ('\b'), se verifica si i es 0. Si i es 0, se rompe del switch. 
                if(i == 0){
                    break;
                } 
                //Si no
                putChar(c); //se imprime el carácter de retroceso y se decrementa i en 1. Esto es para manejar la eliminación de un carácter del buffer.
                i--;
                break;
            default://Para cualquier otro carácter (default)
                if(i == (count -1)){ //se verifica si i es mayor o igual a count y, de ser asi, salimos 
                    break;
                }
                buff[i++] = c;
                putChar(c);
        }
    }
   

    return i; //Retornamos el numero de caracteres copiados en el buffer
}

void intToString(int n, char* s) {
    int i  = 0;
    char flag = 0; //se usará para indicar si el número es negativo.

    if(n < 0)
        flag = 1;//prendo el flag para indicar que el nro es negativo

    do{
        s[i++] = n % 10 + '0';  //adds ASCII value
        n = n / 10;
    }while(n != 0);


    if(flag) { //Si flag es 1 (es decir, si el número original era negativo), añade un carácter '-' a s y avanza i.
        s[i++] = '-';
    }

    s[i]='\0';//Añade un carácter nulo '\0' al final de s para terminar la cadena.

    //Invierte la cadena s
    for (int j = 0; j < (i/2); j++) {
        char temp = s[j];
        s[j] = s[i-j-1];
        s[i - j - 1] = temp;
    }
}

int stringToInt(char* str) {
    int result = 0;     // Almacenará el número resultante
    int sign = 1;       // Para manejar números negativos

    // Si el primer carácter es un signo negativo, ajustamos el signo
    if (*str == '-') {
        sign = -1;
        str++; // Avanzamos al siguiente carácter
    }

    // Recorremos cada carácter de la cadena
    while (*str != '\0') {
        // Aseguramos que el carácter sea un dígito
        if (*str < '0' || *str > '9') {
            return 0; // Si encontramos un carácter no válido, devolvemos 0
        }

        // Convertimos el carácter a su valor numérico
        result = result * 10 + (*str - '0');

        // Avanzamos al siguiente carácter
        str++;
    }

    // Retornamos el resultado multiplicado por el signo
    return result * sign;
}
