#ifndef APPLICATIONS_H
#include <stdint.h>
#define APPLICATIONS_H
#define SLEEP_TIME 5000
#define SHELL_PID 1

typedef enum PState {RUNNING, BLOCKED, READY, ZOMBIE, TERMINATED} PState;

void loop(uint64_t argc, char *argv[]);
int kill(uint64_t argc, char *argv[]);
void nice(uint64_t argc, char *argv[]);
void block(uint64_t argc, char *argv[]);
void play_song(uint64_t argc, char *argv[]);
void play_eliminator(uint64_t argc, char *argv[]);
void time(uint64_t argc, char *argv[]);
void help(uint64_t argc, char *argv[]);
#endif