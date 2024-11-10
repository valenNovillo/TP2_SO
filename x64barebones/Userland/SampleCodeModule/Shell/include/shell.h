#ifndef SHELL_H
#define SHELL_H

#include "../../Library/include/string.h"
#include "music.h"
#include "../../Library/include/stdio1.h"


#define BUFF_SIZE 100
#define SHELL_PIPE_ID 1234

typedef uint16_t (*Commands)(int16_t fds[3], char *args[]);

extern void test_io_exception();

//===================================================================================== PROTOTIPOS ===================================================================================================================

void shell();

uint16_t help_command(int16_t fds[]);

int zoomIn();

int zoomOut();

uint16_t time_command(int16_t fds[]);

int clean();

void ioexception();

void zeroexception();

uint16_t playEliminator(int16_t  fds[]);

uint16_t playSong(int16_t  fds[]);

void processCommand(char * input);

uint16_t findCommand(char * input, int16_t fds[]);

void resetBuffer();

uint16_t test_process(int16_t fds[]);

uint16_t ps_commmand();

uint16_t test_priority(int16_t fds[]);

uint16_t testing_sync(int16_t  fds[]);

uint16_t testing_no_sync(int16_t fds[]);

uint16_t test_mm_command(int16_t  fds[]) ;

uint16_t print_mem_status_command();

uint16_t loop_command(int16_t fds[], char *args[]);

uint16_t kill_command(int16_t fds[], char *args[]);

uint16_t nice_command(int16_t fds[], char *args[]);

uint16_t block_command(int16_t fds[],char *args[]);

uint16_t cat_command(int16_t fds[]);

uint16_t wc_command(int16_t fds[]);

uint16_t filter_command(int16_t fds[]);

//========================================================================================================================================================================================================================


#endif //SHELL_H
