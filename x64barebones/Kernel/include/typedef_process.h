#ifndef TYPEDEF_H
#define TYPEDEF_H

#define STATES 5

typedef enum PState {RUNNING, BLOCKED, READY, ZOMBIE, TERMINATED} PState;

typedef int (*Main)(int argc, char **args);

#endif //TYPEDEF_H