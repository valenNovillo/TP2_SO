#include "include/stdio1.h"

void intToString(int n, char* s);
int readByDelim(int fd, char * buff, int count, char delim);

int getChar() {
    char c = 0;
    int count = 0;
    while(count == 0) 
        count = read(0, &c, 1);
    return count == 1 ? c : -1;
}

int putChar(int ch) {
    int n;
    n = write(1, &ch, 1);
    return n == -1 ? -1 : (unsigned char)ch;
}

int scanf1(char* format, ...) {
    va_list args;
    va_start(args, format); //Inicializa args para que apunte al primer argumento variable después de format

    int readChars = 0;//Esta variable contará el número de caracteres leídos.
    int length; // para almacenar la longitud de la cadena leída.
    char buffer[100]; //para almacenar la entrada leida

    int* argD;      //Puntero a argumentos enteros
    char* sPointer; //Puntero a argumentos de cadena

    int i = 0; //Indice para recorrer la cadena format
    while( format[i] != 0 ){
        if( format[i] == '%') {
            i++;

            length = readByDelim(0, buffer, 100, '\n');//Lee por entrada estandar y lo almacena en el buffer//OJO!(PODRIA SER FD = 1?)
            buffer[length] = 0;//Para terminar la cadena
            readChars += length;//Actualizo la cantidad de caracteres leidos hasta el momento

            putChar(' ');//Imprime un espacio en la salida estándar.

            switch (format[i]) {
                case 'd':
                    argD = va_arg(args, int*); //obtiene el siguiente argumento de la lista (args) como un puntero a int (argD).
                    *argD = 0;
                    for (int j = 0; buffer[j] != 0; ++j) //Convierte la cadena buffer a un entero, almacenando el resultado en argD
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

void printf(char * string, ...) {
    va_list argPointer; // se utilizará para acceder a los argumentos variables.
    va_start(argPointer,string);  //Inicializa argPointer para que apunte al primer argumento variable después de string.

    int i=0; //indice para recorrer la cadena string.
    while(string[i]!=0){ 
        if(string[i] == '%'){   // si tenemos un '%', se trata de un especificador de formato
            char buffInt[20];//para almacenar la representación en cadena de un entero
            char * str; 

            //switch para manejar el carácter siguiente al % en string, que indica el tipo de formato.
            switch (string[i+1]) { 
                case 'd':    // indicador de formato para enteros
                    intToString(va_arg(argPointer, int), buffInt);  // Llama a va_arg para obtener el siguiente argumento de tipo int y convierte este entero a una cadena usando intToString, almacenando el resultado en buffInt
                    printf(buffInt);   //llamado recursivo para imprimir la cadena buffInt
                    break;
                case 's':  // indicador de formato para cadenas
                    str = va_arg(argPointer, char*); //Llama a va_arg para obtener el siguiente argumento de tipo char* y lo almacena en str.
                    printf(str);  // llamado recursivo para imprimir la cadena str
                    break;
            }
            i+=2;// incrementa i en 2 para saltar el % y el carácter del formato.
        }else if(string[i]=='\\') {  //Si el carácter actual es una barra invertida (\\), maneja caracteres especiales (\n, \t, \\)
            switch (string[i+1]) {
                case 'n':
                    putChar('\n'); break; //Si el carácter siguiente es n, imprime un salto de línea ('\n').
                case 't':
                    putChar('\t'); break;//Si el carácter siguiente es t, imprime una tabulación ('\t')
                case '\\':
                    putChar('\\'); break;//Si el carácter siguiente es \\, imprime una barra invertida ('\\').
            }
            i+=2; //incrementa i en 2 para saltar la barra invertida y el carácter especial.
        }else
            putChar(string[i++]);//i no se encuentra un % o una barra invertida (\\), imprime el carácter actual y luego incrementa i en 1.
    }

    va_end(argPointer);//Llama a va_end para limpiar la lista de argumentos variables.
}


void printErr(char * buff) {
    write(2, buff, strlen(buff));
}


int readByDelim(int fd, char * buff, int count, char delim) {
    if(fd != 0){ //Si no es entrada estándar no hay nada que hacer
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
