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