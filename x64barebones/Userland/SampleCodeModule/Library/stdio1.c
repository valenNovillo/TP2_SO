#include "include/stdio1.h"

void intToString(int n, char* s);

static int get_char_fd(int16_t fd) {
    char c = 0;
    int count = 0;
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

int vfscanf(int16_t* fd, char* format, va_list args) {

    int readChars = 0;
    int length;
    char buffer[100];

    int* argD;
    char* sPointer;

    int i = 0;
    while( format[i] != 0 ){
        if( format[i] == '%') {
            i++;

            length = readByDelim(fd, buffer, 100, '\n');
            buffer[length] = 0;
            readChars += length;

            put_char_fd(fd[STDOUT], ' ');

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

    return readChars;//retorna la cantidad de caracteres leídos
}

static void vfprintf(int16_t fd, char *string, va_list argPointer) {
    int i = 0;
    while (string[i] != 0) {
        if (string[i] == '%') {
            char buffInt[20];
            char *str;

            switch (string[i + 1]) {
                case 'd':
                    intToString(va_arg(argPointer, int), buffInt);
                    vfprintf(fd, buffInt, argPointer);
                    break;
                case 's':
                    str = va_arg(argPointer, char *);
                    vfprintf(fd, str, argPointer);
                    break;
            }
            i += 2;
        } else if (string[i] == '\\') {
            switch (string[i + 1]) {
                case 'n':
                    put_char_fd(fd, '\n');
                    break;
                case 't':
                    put_char_fd(fd, '\t');
                    break;
                case '\\':
                    put_char_fd(fd, '\\');
                    break;
            }
            i += 2;
        } else {
            put_char_fd(fd, string[i++]);
        }
    }
}

int scanf1(char* format, ...) {
    va_list argPointer;
    va_start(argPointer, format);
    int cant = vfscanf(get_fds(), format, argPointer);
    va_end(argPointer);
    return cant;
}

void fprintf(int16_t fd, char *string, ...) {
    va_list argPointer;
    va_start(argPointer, string);
    vfprintf(fd, string, argPointer);
    va_end(argPointer);
    put_char_fd(fd, '\0');
}

void printf(char *string, ...) {
    va_list argPointer;
    va_start(argPointer, string);
    vfprintf(get_fds()[STDOUT], string, argPointer);
    va_end(argPointer);
    put_char_fd(get_fds()[STDOUT], '\0');
}


void printErr(char * buff, ...) {
    va_list argPointer;
    va_start(argPointer, buff);
    vfprintf(get_fds()[STDERR], buff, argPointer);
    va_end(argPointer);
    put_char_fd(get_fds()[STDERR], '\0');
}


int readByDelim(int16_t* fd, char * buff, int count, char delim) {

    int i = 0; //para contabilizar los caracteres leidos
    char c; //para almacenar el caracter leido


    while((c = get_char_fd(fd[STDIN])) != delim && c != EOF){ //mientras que el caracter sea distinto del delimitador, seguimos 
        switch (c) {
            case 0: //Si el buffer esta vacío o no se leyó un caracter válido
                break;
            case '\b': //Si c es un carácter de retroceso ('\b'), se verifica si i es 0. Si i es 0, se rompe del switch. 
                if(i == 0){
                    break;
                } 
                //Si no
                put_char_fd(fd[STDOUT], c); //se imprime el carácter de retroceso y se decrementa i en 1. Esto es para manejar la eliminación de un carácter del buffer.
                i--;
                break;
            default://Para cualquier otro carácter (default)
                if(i == (count -1)){ //se verifica si i es mayor o igual a count y, de ser asi, salimos 
                    break;
                }
                buff[i++] = c;
                put_char_fd(fd[STDOUT], c);
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
