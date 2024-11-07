#include "include/shell.h"
#include "include/eliminator.h"
#include "../Test/test.h"
#include "../Library/include/stdio1.h"
#include "../Library/include/unistd1.h"
#include "../Library/include/string.h"
#include "../Library/include/applications.h"
#include <stddef.h>

static void (*commands[])(char *args[]) = {help, zoomIn, zoomOut, time, clean, ioexception, zeroexception, playEliminator,
 playSong, test_process,test_priority, ps_commmand, testing_sync, testing_no_sync, print_mem_status_command, test_mm_command, loop_command, kill_command, nice_command, block_command};

static char* commands_name[] = {"help", "inc", "dec", "time", "clean", "ioexception", "zeroexception",
     "eliminator", "playsong", "test_processes", "test_priority", "ps", "test_sync", "test_no_sync", "mem", "test_mm", "loop", "kill", "nice", "block"};

char buffer[BUFF_SIZE]={0};

void shell()
{
    printf("\n\n\n\n\n");
    setColor(150, 112, 150);
    printf("                WELCOME TO THE BEST OS EVER!! <3 <3             \n");
    musicDispatcher(HIMNO);
    cleanScreen();
    setColor(255, 255, 255);
    printf("\n\nWelcome!\n\nType 'help' to show a list with all the functionalities!\n");
    reading(1);

    while(1) {
        setColor(10, 255, 15);
        printf("\n\n:$> ");
        setColor(255, 255, 255);
        scanf1("%s",buffer);
        findCommand(buffer);
        resetBuffer();
    }   
}

static int hasPipe(int argc){
    for (int i = 0; i < argc; i++) {
        if (strCmp(arguments[i], "|") == 0) {
            return i;
        }
    }
    return -1;
}

void resetBuffer() {
    for(int i=0; i<BUFF_SIZE; i++) {
        buffer[i] = 0;
    }
}


void findCommand(char * input) {
    char *args[10] = {NULL}; 
    char *token = strtok(input, " ");
    int arg_count = 0;

    while (token != NULL && arg_count < 10) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    if (arg_count == 0) {
        printErr("\nNo command entered.\n\n");
        return;
    }

    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strcmp(args[0], commands_name[i]) == 0) {
            commands[i](args + 1); 
            return;
        }
    }
    printErr("\nPlease type a valid command.\n\n");
}


void help() {
    printf("\n");
    printf("The available commands are:\n");

    setColor(250, 255, 0);
    printf("help \n");
    setColor(255, 255, 255);
    printf("--> Displays a menu with all the comands\n");

    setColor(250, 255, 0);
    printf("inc \n");
    setColor(255, 255, 255);
    printf("--> Increases text size on the screen\n");

    setColor(250, 255, 0);
    printf("dec \n");
    setColor(255, 255, 255);
    printf("--> Decreases text size on the screen\n");

    setColor(250, 255, 0);
    printf("eliminator \n");
    setColor(255, 255, 255);
    printf("--> Starts a new game of Eliminator \n");
    
    setColor(250, 255, 0);
    printf("time \n");
    setColor(255, 255, 255);
    printf("--> Displays current time in hh:mm:ss format\n");

    setColor(250, 255, 0);
    printf("clean \n");
    setColor(255, 255, 255);
    printf("--> Erases the content on the screen\n");

    setColor(250, 255, 0);
    printf("ioexception \n");
    setColor(255, 255, 255);
    printf("--> Produces an invalid opcode exception\n");

    setColor(250, 255, 0);
    printf("zeroexception \n");
    setColor(255, 255, 255);
    printf("--> Produces a zero division exception\n");

    setColor(250, 255, 0);
    printf("playsong \n");
    setColor(255, 255, 255);
    printf("--> Shares a list of songs for playing\n");

    setColor(250, 255, 0);
    printf("test_processes \n");
    setColor(255, 255, 255);
    printf("--> Checks the management of multiple processes\n");

    setColor(250, 255, 0);
    printf("test_priority \n");
    setColor(255, 255, 255);
    printf("--> Checks the management of multiple priorities\n");

    setColor(250, 255, 0);
    printf("test_sync \n");
    setColor(255, 255, 255);
    printf("--> Checks the syncronization between processes\n");

    setColor(250, 255, 0);
    printf("test_no_sync \n");
    setColor(255, 255, 255);
    printf("--> Checks the result withouth sync between processes\n");

    setColor(250, 255, 0);
    printf("test_mm \n");
    setColor(255, 255, 255);
    printf("--> Checks the memory management\n");

    setColor(250, 255, 0);
    printf("mem \n");
    setColor(255, 255, 255);
    printf("--> Prints the memory status\n");

    setColor(250, 255, 0);
    printf("ps \n");
    setColor(255, 255, 255);
    printf("--> Prints list of all current processes with their properties\n");

    setColor(250, 255, 0);
    printf("loop \n");
    setColor(255, 255, 255);
    printf("--> Prints current process ID with a greeting\n");

    setColor(250, 255, 0);
    printf("kill <ID> \n");
    setColor(255, 255, 255);
    printf("--> Kills the process <ID> \n");

    setColor(250, 255, 0);
    printf("nice <ID> <PRIORITY> \n");
    setColor(255, 255, 255);
    printf("--> Changes the process <ID> priority to <PRIORITY> \n");

    setColor(250, 255, 0);
    printf("block <ID> \n");
    setColor(255, 255, 255);
    printf("--> Changes the process <ID> state between blocked and ready\n");

    setColor(133, 21, 199);
    printf("registers \n");
    setColor(255, 255, 255);
    printf("--> To see the current state of the processor registers, please press CTRL + R\n");
}

void zoomIn() {
    sizeInc();
}
    
void zoomOut() {
    sizeDec();
}

void time() {
    char time[10];
    getTime(time);
    printf("\n\n%s\n", time);
}

void clean() {
    cleanScreen();
}

void ioexception() {
    test_io_exception();
}

void zeroexception() {
    int i = 5;
    i /= 0;
    return;
}

void playEliminator() {
    reading(0);
    eliminator();
    reading(1);
}

void playSong()
{
    printf("\n\n");
    printf("Choose a song by number:\n\n");

    setColor(0, 255, 255);
    printf("0: Himno Argentino (which is not actually Himno Argentino) \n\n");

    printf("1: startUpMusic\n\n");

    printf("2: Mario Brothers song \n\n");

    printf("3: Happy Birthday \n\n");

    printf("4: Arrorro mi Nino \n\n");

    printf("5: Smoke On Water  \n\n");

    printf("6: Try of Seven Nation Army (Almost) \n\n");

    printf("7: Game Music \n\n");

    setColor(255, 255, 255);

    int song;
    scanf1("%d",&song);
    if(song<0 || song>=MAX_SONGS) {
        printErr("\n\nPlease choose a valid song.\n\n");
        return;
    }
    musicDispatcher(song);
}

void test_process() {
    char* argv[] = {MAX_PROCESSES, 0};
    int16_t fds[] = {NO_INPUT, STDOUT, STDERR};
    create_process((Main)test_processes, argv, "test_process", 2, fds);
}

void test_priority() {
    char* argv[] = {0};
    int16_t fds[] = {NO_INPUT, STDOUT, STDERR};
    create_process((Main)test_prio, argv, "test_priority", 2, fds);
}

void ps_commmand() {
    ps();
}

void testing_sync() {
    char* argv[] = {"3", "1", 0}; //{n, use_sem, 0}
    int16_t fds[] = {STDIN, STDOUT, STDERR};
    create_process((Main)test_sync, argv, "test_sync", 1, fds);
}

void testing_no_sync() {
    char* argv[] = {"3", "0", 0}; //{n, use_sem, 0}
    int16_t fds[] = {NO_INPUT, STDOUT, STDERR};
    create_process((Main)test_sync, argv, "test_no_sync", 1, fds);
}

void print_mem_status_command() {
    print_mem_status();
}

void test_mm_command() {
    char* argv[] = {MEMORY_SIZE, 0};
    int16_t fds[] = {NO_INPUT, STDOUT, STDERR};
    create_process((Main)test_mm, argv, "test_mm", 5, fds);
}

void loop_command() {
    char* argv[] = {0};
    int16_t fds[] = {NO_INPUT, STDOUT, STDERR};
    create_process((Main)loop, argv, "loop", 3, fds);
}

void kill_command(char *args[]) {
    kill(args);
}

void nice_command(char *args[]) {
    nice(args);
}

void block_command(char *args[]) {
   block(args);
}




 