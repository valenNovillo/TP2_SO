#ifndef SHELL_H
#define SHELL_H

#include "../../Library/include/stdio1.h"
#include "../../Library/include/unistd1.h"
#include "../../Library/include/string.h"
#include "../../Library/include/time.h"
#include "music.h"

#define BUFF_SIZE 100

extern void test_io_exception();

//función que ejecuta todo lo correspondiente a la terminal
void shell();

#endif //SHELL_H
