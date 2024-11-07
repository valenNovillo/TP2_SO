#ifndef SHELL_H
#define SHELL_H

#include "../../Library/include/string.h"
#include "../../Library/include/time.h"
#include "music.h"

#define BUFF_SIZE 100

typedef enum PState {RUNNING, BLOCKED, READY, ZOMBIE, TERMINATED} PState;

extern void test_io_exception();

//===================================================================================== PROTOTIPOS ===================================================================================================================

void shell();

void help();

void zoomIn();

void zoomOut();

void time();

void clean();

void ioexception();

void zeroexception();

void playEliminator();

void playSong();

void findCommand(char * input);

void resetBuffer();

void test_process();

void ps_commmand();

void test_priority();

void testing_sync();

void testing_no_sync();

void test_mm_command();

void print_mem_status_command();

void loop_command();

void kill_command();

void nice_command(char *args[]);

void block_command(char *args[]);

//========================================================================================================================================================================================================================


#endif //SHELL_H
