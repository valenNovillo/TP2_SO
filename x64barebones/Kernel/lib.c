#include <stdint.h>

#include "include/lib.h"
#include "../Drivers/include/fileDescriptors.h"
#include "../Drivers/include/videoDriver.h"
#include "include/pipe.h"

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}


int intToString(int n, char* s) {
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

	return i;
}


// Función para invertir una cadena de caracteres
void reverseString(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Función para convertir un número decimal a hexadecimal
int decimalToHexadecimal(uint64_t decimal, char* buffer, int bufferSize) {
    const char hexDigits[] = "0123456789ABCDEF";
    int index = 0;

    // Manejar el caso cuando el número es 0
    if (decimal == 0) {
        if (bufferSize > 1) {
            buffer[index++] = '0';
        }
        buffer[index] = '\0';
        return index;
    }

    // Convertir el número decimal a hexadecimal
    while (decimal != 0 && index < bufferSize - 1) {
        int remainder = decimal % 16;
        buffer[index++] = hexDigits[remainder];
        decimal /= 16;
    }

    // Añadir el carácter nulo al final de la cadena
    buffer[index] = '\0';

    // Invertir la cadena para obtener el valor correcto
    reverseString(buffer, index);

    // Devolver la longitud de la cadena hexadecimal
    return index;
}

size_t strlen(const char *cadena) {
    int len = 0;
    while(cadena[len++] != 0);
    return len;
}


int count_args(void **array){
    int count = 0;
    while (array[count] != NULL){
        count++;
    }
    return count;
}

ssize_t write(int16_t fd, char* buff, uint64_t count){
      if (fd == STDOUT || fd == STDERR) {
        putString(fd, buff, count);
    }else if(fd > STDERR){
        return write_on_file(fd, buff, count);
    }
    return -1;
}