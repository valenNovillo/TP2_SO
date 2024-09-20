#include "include/string.h"

size_t strlen(const char *cadena) {
    int len = 0;
    while(cadena[len++] != 0);
    return len;
}

int strcmp(const char *cadena1, const char *cadena2) {
    int i = 0;
    while(cadena1[i] && (cadena1[i] == cadena2[i]))
        i++;
    return cadena1[i] - cadena2[i];
}
