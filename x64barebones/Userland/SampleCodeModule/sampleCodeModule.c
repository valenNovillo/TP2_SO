// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Shell/include/shell.h"
#include <stddef.h>

int main(int argc, char* argv[]){
    char* args[] = {NULL};
    int16_t fds[3] = {STDIN, STDOUT, STDERR};
    create_process((Main)shell, args, "sh", 5, fds);
    while(1){
       _hlt();
    }
    return 0;
}