#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
int intToString(int n, char* s);
int decimalToHexadecimal(uint64_t decimal, char* buffer, int bufferSize);
void reverseString(char* str, int length);

#endif