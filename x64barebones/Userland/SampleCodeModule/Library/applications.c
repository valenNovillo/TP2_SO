#include "./include/applications.h"
#include "../Library/include/stdio1.h"
#include "../Library/include/unistd1.h"
#include "../Shell/include/eliminator.h"
#include "../Library/include/time.h"
#include "./include/lib.h"
#include <stddef.h>

int loop(uint64_t argc, char *argv[]) {
    int pid = get_pid(); 

    while (1) {
        printf("\nHi! I'm process %d and I'll be looping every %d seconds\n", pid, SLEEP_TIME/1000);
        sleep(SLEEP_TIME);  
    }
}

void kill(int16_t fds[], char *argv[]) {
    if (argv[0] == NULL) {
        printf("\nPlease specify a process ID to kill.\n");
        return;
    }

    int pid = stringToInt(argv[0]); 

    if(kill_process(pid) < 0) {
        printf("\nCould not kill process %d.\n", pid);
        return;
    }

    printf("\nProcess %d has been killed.\n", pid);
}

void nice(int16_t fds[], char *argv[]) {
    if (argv[0] == NULL) {
        printf("\nPlease specify the process ID you want to change its priority.\n");
        return;
    }

    if (argv[1] == NULL) {
        printf("\nPlease specify the new PRIORITY you want to set.\n");
        return;
    }

    int pid = stringToInt(argv[0]); 
    int priority = stringToInt(argv[1]); 

    if(stringToInt(argv[0]) == SHELL_PID) {
        printf("\nCould not change process %d priority to %d.\n", pid , priority);
        return;
    }

     if(stringToInt(argv[0]) == DEFAULT) {
        printf("\nCould not change process %d priority to %d.\n", pid, priority);
        return;
    }

    if(set_priority(pid, priority) < 0) {
        printf("\nCould not change process %d priority to %d.\n", pid, priority);
        return;
    }

    printf("\nProcess %d now has priority %d.\n", pid, priority);
}

void block(int16_t fds[],char *argv[]) {
    if (argv[0] == NULL) {
        printf("\nPlease specify the process ID you want to change its state\n");
        return;
    }

    int pid = stringToInt(argv[0]); 
    
    if(stringToInt(argv[0]) == SHELL_PID) {
        printf("\nCould not change process %d state\n", pid);
        return;
    }

    PState state = get_state(pid);
    int ret = 0;

    if(state == BLOCKED) {
        ret = unblock_process(pid);
    } else if (state == READY || state == RUNNING) {
        ret = block_process(pid);
    } else {
        printf("\nCould not change process %d state\n", pid);
        return;
    } 

    if(ret == -1) {
        printf("\nCould not change process %d state\n", pid);
        return;
    }
      
    PState new_state = get_state(pid);  

    printf("\nProcess %d changed to state %s \n", pid, new_state == BLOCKED ? "BLOCKED" : (new_state == READY ? "READY" : "RUNNING"));
}

int play_song(uint64_t argc, char *argv[]) {
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
        printf("\n\nPlease choose a valid song.\n\n");
        return -1;
    }
    musicDispatcher(song);
    return 0;
}

int play_eliminator(uint64_t argc, char *argv[]) {
    reading(0);
    eliminator();
    reading(1);
    return 0;
}

void time(int16_t fds[]) {
    char time[10];
    getTime(time);
    printf("\n\n%s\n", time);
}

void help(int16_t fds[]) {
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

/*int cat(uint64_t argc, char *argv[]) {
    char buffer[MAX_BUFF];
	while(1) {
        scanf1("%s",buffer);
        printf("%d\n", buffer);
    }
}*/

int cat(uint64_t argc, char *argv[]) {
    char buffer[MAX_BUFFER_SIZE];
    int index = 0;
    char c;

    while ((c = getChar()) != EOF) {
        // Si el carácter es Enter, imprime el contenido del buffer y reinícialo
        if (c == '\n') {
            buffer[index] = '\0';  // Agrega el carácter nulo para marcar el fin de la cadena
            printf("%s",buffer);          // Imprime el buffer
            index = 0;             // Reinicia el índice para la próxima línea
        } else if (index < MAX_BUFFER_SIZE - 1) {
            // Almacena el carácter en el buffer si no se ha excedido el tamaño máximo
            buffer[index++] = c;
        }
    }

    return 0;
}

