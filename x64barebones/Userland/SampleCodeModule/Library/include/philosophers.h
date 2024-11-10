#ifndef PHILOSOPHERS_H
#include <stdint.h>

int init_philos_restaurant(uint64_t argc, char *argv[]);
int philosopher(uint64_t argc, char *argv[]);
void eat(int philo_id);

#endif