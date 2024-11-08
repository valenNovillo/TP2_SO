#ifndef APPLICATIONS_H
#define APPLICATIONS_H
#define SLEEP_TIME 5000
#define SHELL_PID 1

typedef enum PState {RUNNING, BLOCKED, READY, ZOMBIE, TERMINATED} PState;

void loop();
void kill(char *args[]);
void nice(char *args[]);
void block(char *args[]);
void play_song();
void play_eliminator();
void time();
void help();
#endif