// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/shell.h"
#include "../Test/test.h"
#include "../Library/include/stdio1.h"
#include "../Library/include/unistd1.h"
#include "../Library/include/string.h"
#include "../Library/include/applications.h"
#include "../Library/include/philosophers.h"
#include <stddef.h>

static Commands commands[] = {(Commands)help_command, (Commands)zoomIn, (Commands)zoomOut, (Commands)time_command, (Commands)clean, (Commands)ioexception, (Commands)zeroexception, (Commands)playEliminator,
 (Commands)playSong,(Commands) test_process,(Commands)test_priority, (Commands)ps_commmand, (Commands)testing_sync, (Commands)testing_no_sync, (Commands)print_mem_status_command, (Commands)test_mm_command, 
 (Commands)loop_command, (Commands)kill_command, (Commands)nice_command, (Commands)block_command, (Commands)cat_command, (Commands)wc_command, (Commands)filter_command, (Commands)phylo_command};

static char* commands_name[] = {"help", "inc", "dec", "time", "clean", "ioexception", "zeroexception",
     "eliminator", "playsong", "test_processes", "test_priority", "ps", "test_sync", "test_no_sync", "mem", "test_mm", "loop", "kill", "nice", "block", "cat", "wc", "filter", "phylo"};

static char* no_pipe_command[] = {"inc", "dec", "clean", "ioexception", "zeroexception", "eliminator", "playSong", "loop"};


char buffer[BUFF_SIZE]={0};

void shell(){
    printf("\n\n\n\n\n");
    setColor(150, 112, 150);
    printf("                WELCOME TO THE BEST OS EVER!! <3 <3             \n");
    musicDispatcher(HIMNO);
    cleanScreen();
    setColor(255, 255, 255);
    printf("\n\nWelcome!\n\nType 'help' to show a list with all the functionalities!\n");
    reading(1);

    while(1){
        setColor(10, 255, 15);
        printf("\n\n:$> ");
        setColor(255, 255, 255);
        scanf1("%s",buffer);
        processCommand(buffer);
        resetBuffer();
    }   
}


void resetBuffer(){
    for(int i=0; i<BUFF_SIZE; i++){
        buffer[i] = 0;
    }
}


void processCommand(char * input){

    if(my_strchr(input, '|') != NULL){
        char * izq = strtok(input, "|");
        char * der = strtok(NULL, "|");
        
        if (izq == NULL || der == NULL){
            printErr("\nInvalid command entered.\n\n");
            return;
        }

        izq[strlen(izq) - 1] = '\0';
        der += 1;

        for (int i = 0; i < sizeof(no_pipe_command) / sizeof(char*); i++){
            if (strcmp(izq, no_pipe_command[i]) == 0 || strcmp(der, no_pipe_command[i]) == 0){
                printErr("\nImpossible to pipe one of the commands\n");
                return;
            }
        }

        int r_pipe_fd = open_pipe(SHELL_PIPE_ID, 'r');
        int w_pipe_fd = open_pipe(SHELL_PIPE_ID, 'w'); 

        if (r_pipe_fd == -1 || w_pipe_fd == -1){
            printErr("Error while creating pipe");
            return;
        }

        int16_t fds_readers[3] = {r_pipe_fd, STDOUT, STDERR};
        int16_t fds_writers[3] = {STDIN, w_pipe_fd, STDERR};

        int16_t pid_reader = findCommand(der, fds_readers);
        
        int16_t pid_writer = findCommand(izq, fds_writers);

        if (pid_reader == -1){
            kill_process(pid_writer);
            close_pipe_for_pid(SHELL_PIPE_ID, (pid_writer == 0 ? SHELL_PID : pid_writer));
            return;
            
        } else if (pid_writer == -1){
            kill_process(pid_reader);
            close_pipe_for_pid(SHELL_PIPE_ID, pid_reader);
            return;
        }

        wait_pid(pid_writer);
        close_pipe_for_pid(SHELL_PIPE_ID, (pid_writer == 0 ? SHELL_PID : pid_writer));
        wait_pid(pid_reader);
        close_pipe_for_pid(SHELL_PIPE_ID, pid_reader);


    }
    else{
        int16_t fds[3] = {STDIN, STDOUT, STDERR};
        findCommand(input, fds);
    }
}


int16_t findCommand(char * input, int16_t fds[]){
    if(fds[0] == STDIN && input[0] == '&'){
        fds[0] = NO_INPUT;
        input += 1;
    }
    
    char *args[10] = {NULL}; 
    char *token = strtok(input, " ");
    int arg_count = 0;

    while (token != NULL && arg_count < 10){
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    if (arg_count == 0){
        printErr("\nNo command entered.\n\n");
        return -1;
    }

    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++){
        if (strcmp(args[0], commands_name[i]) == 0){
            return commands[i](fds, args + 1);
        }
    }
    printErr("\nPlease type a valid command.\n\n");
    return -1;
}


int16_t help_command(int16_t fds[]){
    help(fds);
    return 0;
}

int zoomIn(){
    sizeInc();
    return 0;
}
    
int zoomOut(){
    sizeDec();
    return 0;
}

int16_t time_command(int16_t fds[]){
    time(fds);
    return 0;
}

int clean(){
    cleanScreen();
    return 0;
}

void ioexception(){
    test_io_exception();
}

void zeroexception(){
    int i = 5;
    i /= 0;
    return;    
}

int16_t playEliminator(int16_t fds[]){
  char* argv[] = {0};
  return create_process((Main)play_eliminator, argv, "play_eliminator", 2, fds);
}

int16_t playSong(int16_t fds[]){
    char* argv[] = {0};
    return create_process((Main)play_song, argv, "play_song", 2, fds);
}

int16_t test_process(int16_t fds[]){
    char* argv[] = {MAX_PROCESSES, 0};
    return create_process((Main)test_processes, argv, "test_process", 2, fds);
}

int16_t test_priority(int16_t fds[]){
    char* argv[] = {0};
    return create_process((Main)test_prio, argv, "test_priority", 2, fds);
}

int16_t ps_commmand(int16_t fds[]){
    return ps(fds);
}

int16_t testing_sync(int16_t fds[]){
    char* argv[] = {"3", "1", 0}; //{n, use_sem, 0}
    return create_process((Main)test_sync, argv, "test_sync", 1, fds);
}

int16_t testing_no_sync(int16_t fds[]){
    char* argv[] = {"3", "0", 0}; //{n, use_sem, 0}
    return create_process((Main)test_sync, argv, "test_no_sync", 1, fds);
}

int16_t print_mem_status_command(int16_t fds[]){
    return print_mem_status(fds);
}

int16_t test_mm_command(int16_t fds[]){
    char* args[] = {MEMORY_SIZE, 0};
    return create_process((Main)test_mm, args, "test_mm", 5, fds);
}

int16_t loop_command(int16_t  fds[], char *args[]){
   return create_process((Main)loop, args, "loop", 3, fds);
}

int16_t kill_command(int16_t fds[], char *args[]){
    kill(fds,args);
    return 0;
}

int16_t nice_command(int16_t fds[], char *args[]){
   nice(fds,args);
   return 0;
}

int16_t block_command(int16_t fds[],char *args[]){
    block(fds,args);
    return 0;
}

int16_t cat_command(int16_t fds[]){
    char* argv[] = {0};
    return create_process((Main)cat, argv, "cat", 2, fds);
}

int16_t wc_command(int16_t fds[]){
    char* argv[] = {0};
    return create_process((Main)wc, argv, "wc", 2, fds);
}

int16_t filter_command(int16_t fds[]){
    char* argv[] = {0};
    return create_process((Main)filter, argv, "filter", 2, fds);
}

int16_t phylo_command(int16_t fds[]){
    char* argv[] = {0};
    return create_process((Main)init_philos_restaurant, argv, "init_philos_restaurant", 2, fds);
}