#include "./include/applications.h"
#include "../Library/include/stdio1.h"
#include "../Library/include/unistd1.h"
#include "../Shell/include/eliminator.h"
#include "../Library/include/time.h"
#include "./include/lib.h"
#include <stddef.h>

int loop(uint64_t argc, char *argv[]){
    int pid = get_pid(); 

    while (1){
        printf("\nHi! I'm process %d and I'll be looping every %d seconds\n", pid, SLEEP_TIME/1000);
        sleep(SLEEP_TIME);  
    }
}

void kill(int16_t fds[], char *argv[]){
    if (argv[0] == NULL){
        printf("\nPlease specify a process ID to kill.\n");
        return;
    }

    int pid = stringToInt(argv[0]); 

    if(kill_process(pid) < 0){
        printf("\nCould not kill process %d.\n", pid);
        return;
    }

    printf("\nProcess %d has been killed.\n", pid);
}

void nice(int16_t fds[], char *argv[]){
    if (argv[0] == NULL){
        printf("\nPlease specify the process ID you want to change its priority.\n");
        return;
    }

    if (argv[1] == NULL){
        printf("\nPlease specify the new PRIORITY you want to set.\n");
        return;
    }

    int pid = stringToInt(argv[0]); 
    int priority = stringToInt(argv[1]); 

    if(stringToInt(argv[0]) == SHELL_PID){
        printf("\nCould not change process %d priority to %d.\n", pid , priority);
        return;
    }

     if(stringToInt(argv[0]) == DEFAULT){
        printf("\nCould not change process %d priority to %d.\n", pid, priority);
        return;
    }

    if(set_priority(pid, priority) < 0){
        printf("\nCould not change process %d priority to %d.\n", pid, priority);
        return;
    }

    printf("\nProcess %d now has priority %d.\n", pid, priority);
}

void block(int16_t fds[],char *argv[]){
    if (argv[0] == NULL) {
        printf("\nPlease specify the process ID you want to change its state\n");
        return;
    }

    int pid = stringToInt(argv[0]); 
    
    if(stringToInt(argv[0]) == SHELL_PID){
        printf("\nCould not change process %d state\n", pid);
        return;
    }

    PState state = get_state(pid);
    int ret = 0;

    if(state == BLOCKED){
        ret = unblock_process(pid);
    } else if (state == READY || state == RUNNING){
        ret = block_process(pid);
    } else{
        printf("\nCould not change process %d state\n", pid);
        return;
    } 

    if(ret == -1){
        printf("\nCould not change process %d state\n", pid);
        return;
    }
      
    PState new_state = get_state(pid);  

    printf("\nProcess %d changed to state %s \n", pid, new_state == BLOCKED ? "BLOCKED" : (new_state == READY ? "READY" : "RUNNING"));
}

int play_song(uint64_t argc, char *argv[]){
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
    if(song<0 || song>=MAX_SONGS){
        printf("\n\nPlease choose a valid song.\n\n");
        return -1;
    }
    musicDispatcher(song);
    return 0;
}

int play_eliminator(uint64_t argc, char *argv[]){
    reading(0);
    eliminator();
    reading(1);
    return 0;
}

void time(int16_t fds[]){
    char time[10];
    getTime(time);
    fprintf(fds[STDOUT],"\n\n%s\n", time);
}

void help(int16_t fds[]){
    fprintf(fds[STDOUT],"\n");
    fprintf(fds[STDOUT],"The available commands are:\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"help \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Displays a menu with all the comands\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"inc \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Increases text size on the screen\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"dec \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Decreases text size on the screen\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"eliminator \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Starts a new game of Eliminator \n");
    
    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"time \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Displays current time in hh:mm:ss format\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"clean \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Erases the content on the screen\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"ioexception \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Produces an invalid opcode exception\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"zeroexception \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Produces a zero division exception\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"playsong \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Shares a list of songs for playing\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"test_processes \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Checks the management of multiple processes\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"test_priority \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Checks the management of multiple priorities\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"test_sync \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Checks the syncronization between processes\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"test_no_sync \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Checks the result withouth sync between processes\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"test_mm \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Checks the memory management\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"mem \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Prints the memory status\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"ps \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Prints list of all current processes with their properties\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"loop \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Prints current process ID with a greeting\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"kill <ID> \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Kills the process <ID> \n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"nice <ID> <PRIORITY> \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Changes the process <ID> priority to <PRIORITY> \n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"block <ID> \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Changes the process <ID> state between blocked and ready\n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"cat \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Prints the stdin as it receives it \n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"wc \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Counts the amount of lines that input has \n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"filter \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Filter vocals of input \n");

    setColor(250, 255, 0);
    fprintf(fds[STDOUT],"philosophers \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> Simulates the philosofers problem \n");

    setColor(133, 21, 199);
    fprintf(fds[STDOUT],"registers \n");
    setColor(255, 255, 255);
    fprintf(fds[STDOUT],"--> To see the current state of the processor registers, please press CTRL + R\n"); 
}

int cat(uint64_t argc, char *argv[]){
    char buffer[MAX_BUFFER_SIZE] = {0};
    int index = 0;
    int c;
    printf("\n");
    if(get_fds()[STDIN] != STDIN){
        while ((c = getChar()) != EOF){
            buffer[index++] = c;
        }
        printf("%s\n", buffer);
    } else{
        while ((c = getChar()) != EOF){
            if (c == '\n'){
                printf("\n");
                buffer[index] = '\0';
                printf("%s\n", buffer);
                index = 0;
            }
            else if (index < MAX_BUFFER_SIZE - 1){
                putChar(c);
                buffer[index++] = c;
            }
        }
    }


    return 0;
}

int wc(uint64_t argc, char *argv[]){
    char c;
    int lines = 0;
    printf("\n");
    
    char pipe_flag = (get_fds()[STDIN] == STDIN);

    while ((c = getChar()) != EOF){
        if(pipe_flag){
            putChar(c);
        }
        if (c == '\n'){
            lines++;
        }
    }

    printf("\nTotal lines: %d\n", lines);
    return 0;
}

int filter(uint64_t argc, char *argv[]){
    char buffer[MAX_BUFFER_SIZE] = {0};
    int index = 0;
    int c;

    printf("\n");
    if(get_fds()[STDIN] != STDIN){
        while ((c = getChar()) != EOF){
            if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
                c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U'){
                buffer[index++] = c; 
            }
        }
        printf("%s\n", buffer);
    } else {
        while ((c = getChar()) != EOF){
            if (c == '\n') {
                printf("\n");  
                buffer[index] = '\0';      
                for (int i = 0; i < index; i++){
                    char ch = buffer[i];
                    if (ch != 'a' && ch != 'e' && ch != 'i' && ch != 'o' && ch != 'u' &&
                        ch != 'A' && ch != 'E' && ch != 'I' && ch != 'O' && ch != 'U'){
                        putChar(ch);  
                    }
                }
                printf("\n");  

                index = 0; 
            } else if (index < MAX_BUFFER_SIZE - 1){
                putChar(c);  
                buffer[index++] = c;  
            }
        }
    }

    return 0;
}
