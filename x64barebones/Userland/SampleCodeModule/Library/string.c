#include "include/string.h"
#include <stddef.h>

size_t strlen(const char *cadena){
    int len = 0;
    while(cadena[len] != 0){
        len++;
    }
    return len;
}

int strcmp(const char *cadena1, const char *cadena2){
    int i = 0;
    while(cadena1[i] && (cadena1[i] == cadena2[i])){
        i++;
    }
    return cadena1[i] - cadena2[i];
}


char* my_strchr(const char* str, char character){
    // Recorre la cadena hasta encontrar el carácter o el fin de la cadena
    while (*str != '\0'){
        if (*str == character){
            return (char*)str;  // Devuelve un puntero a la posición del carácter
        }
        str++;
    }

    // Si el carácter no se encuentra, devuelve NULL
    return NULL;
}


char* strtok(char* str, const char* delim){
    static char* next_token = NULL; // Puntero estático para recordar la posición
    char* token;

    // Usa la cadena proporcionada si es la primera llamada, si no, continúa desde donde quedó
    if (str != NULL){
        next_token = str;
    }

    // Si ya estamos al final de la cadena, devuelve NULL
    if (next_token == NULL || *next_token == '\0'){
        return NULL;
    }

    // Ignora cualquier carácter delimitador al principio
    while (*next_token != '\0' && my_strchr(delim, *next_token) != NULL){
        next_token++;
    }

    // Si llegamos al final después de los delimitadores, devuelve NULL
    if (*next_token == '\0'){
        return NULL;
    }

    // Apunta al comienzo del token
    token = next_token;

    // Busca el final del token
    while (*next_token != '\0' && my_strchr(delim, *next_token) == NULL){
        next_token++;
    }

    // Si encontramos un delimitador, reemplázalo por '\0' y avanza el puntero
    if (*next_token != '\0'){
        *next_token = '\0';
        next_token++;
    }

    return token;
}

