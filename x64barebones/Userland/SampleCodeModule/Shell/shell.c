#include "include/shell.h"
#include "include/eliminator.h"
#include "../Test/test.h"

//vector de punteros a funcion para ejecutar cada comando
static void (*commands[])() = {help, zoomIn, zoomOut, time, clean, ioexception, zeroexception, playEliminator, playSong, test_process, ps_commmand};

//buffer de lo que ingresan en la terminal
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

void resetBuffer() {
    for(int i=0; i<BUFF_SIZE; i++)
    {
        buffer[i] = 0;
    }
}


void findCommand(char * buffer) {
    if(strcmp(buffer, "help") == 0)
        return commands[0]();
    if(strcmp(buffer, "inc") == 0)
       return commands[1]();
    if(strcmp(buffer, "dec") == 0)
        return commands[2]();   
    if(strcmp(buffer, "time") == 0)
        return commands[3]();      
    if(strcmp(buffer, "clean") == 0)
        return commands[4]();  
    if(strcmp(buffer, "ioexception") == 0)
        return commands[5]();  
    if(strcmp(buffer, "zeroexception") == 0)
        return commands[6]();  
    if(strcmp(buffer, "eliminator") == 0)
        return commands[7]();
    if(strcmp(buffer, "playsong") == 0)
        return commands[8]();
    if(strcmp(buffer, "test_processes") == 0)
        return commands[9]();
    if(strcmp(buffer, "ps") == 0)
        return commands[10]();
    else {
        printErr("\nPlease type a valid command.\n\n");
    }
}


void help() {
    printf("\n\n");
    printf("The available commands are:\n\n");

    setColor(250, 255, 0);
    printf("help \n\n");
    setColor(255, 255, 255);
    printf("--> Displays a menu with all the comands\n\n");

    setColor(250, 255, 0);
    printf("inc \n\n");
    setColor(255, 255, 255);
    printf("--> Increases text size on the screen\n\n");

    setColor(250, 255, 0);
    printf("dec \n\n");
    setColor(255, 255, 255);
    printf("--> Decreases text size on the screen\n\n");

    setColor(250, 255, 0);
    printf("eliminator \n\n");
    setColor(255, 255, 255);
    printf("--> Starts a new game of Eliminator \n\n");
    
    setColor(250, 255, 0);
    printf("time \n\n");
    setColor(255, 255, 255);
    printf("--> Displays current time in hh:mm:ss format\n\n");

    setColor(250, 255, 0);
    printf("clean \n\n");
    setColor(255, 255, 255);
    printf("--> Erases the content on the screen\n\n");

    setColor(250, 255, 0);
    printf("ioexception \n\n");
    setColor(255, 255, 255);
    printf("--> Produces an invalid opcode exception\n\n");

    setColor(250, 255, 0);
    printf("zeroexception \n\n");
    setColor(255, 255, 255);
    printf("--> Produces a zero division exception\n\n");

    setColor(250, 255, 0);
    printf("playsong \n\n");
    setColor(255, 255, 255);
    printf("--> Shares a list of songs for playing\n\n");

    setColor(250, 255, 0);
    printf("test_processes \n\n");
    setColor(255, 255, 255);
    printf("--> Checks the management of multiple processes\n\n");

    setColor(133, 21, 199);
    printf("registers \n\n");
    setColor(255, 255, 255);
    printf("--> To see the current state of the processor registers, please press CTRL + R\n\n");
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
    char* argv[1];
    argv[0] = MAX_PROCESSES;
    int16_t fds[] = {NO_INPUT, STDOUT, STDERR};
    create_process(test_processes, argv, "create_process", 1, fds);
}

void test_priority() {
    //create_process()
}

void ps_commmand() {
    ps();
}



 