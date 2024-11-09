#ifndef APPLICATIONS_H
#include <stdint.h>
#define APPLICATIONS_H
#define SLEEP_TIME 5000
#define SHELL_PID 1
#define DEFAULT 0
#define MAX_BUFFER_SIZE 1024

typedef enum PState {RUNNING, BLOCKED, READY, ZOMBIE, TERMINATED} PState;

int loop(uint64_t argc, char *argv[]);
void kill(int16_t fds[], char *argv[]);
void nice(int16_t fds[], char *argv[]);
void block(int16_t fds[],char *argv[]);
int play_song(uint64_t argc, char *argv[]);
int play_eliminator(uint64_t argc, char *argv[]);
void time(int16_t fds[]);
void help(int16_t fds[]);
int cat(uint64_t argc, char *argv[]);
#endif