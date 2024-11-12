#ifndef SHELL_H
#define SHELL_H

#include "../../Library/include/string.h"
#include "music.h"
#include "../../Library/include/stdio1.h"


#define BUFF_SIZE 100
#define SHELL_PIPE_ID 1234

typedef int16_t (*Commands)(int16_t fds[3], char *args[]);

extern void test_io_exception();

//===================================================================================== PROTOTIPOS ===================================================================================================================

void shell();

int16_t help_command(int16_t fds[]);

int zoomIn();

int zoomOut();

int16_t time_command(int16_t fds[]);

int clean();

void ioexception();

void zeroexception();

int16_t playEliminator(int16_t  fds[]);

int16_t playSong(int16_t  fds[]);

void processCommand(char * input);

int16_t findCommand(char * input, int16_t fds[]);

void resetBuffer();

int16_t test_process(int16_t fds[]);

int16_t ps_commmand(int16_t fds[]);

int16_t test_priority(int16_t fds[]);

int16_t testing_sync(int16_t  fds[]);

int16_t testing_no_sync(int16_t fds[]);

int16_t test_mm_command(int16_t  fds[]) ;

int16_t print_mem_status_command(int16_t fds[]);

int16_t loop_command(int16_t fds[], char *args[]);

int16_t kill_command(int16_t fds[], char *args[]);

int16_t nice_command(int16_t fds[], char *args[]);

int16_t block_command(int16_t fds[],char *args[]);

int16_t cat_command(int16_t fds[]);

int16_t wc_command(int16_t fds[]);

int16_t filter_command(int16_t fds[]);

int16_t phylo_command(int16_t fds[]);

//========================================================================================================================================================================================================================


#endif //SHELL_H
