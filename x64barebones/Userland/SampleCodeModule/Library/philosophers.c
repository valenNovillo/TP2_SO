#include <stdint.h>
#include "../Library/include/unistd1.h"
#include "../Library/include/stdio1.h"
#include "../Library/include/philosophers.h"
#include <stddef.h>
#define MIN_PHILOS 5
#define MAX_PHILOS 10
#define THINKING 0
#define EATING 1
#define THINKING_TIME 2000

static char *philos[MAX_PHILOS] = {
    "philo00", "philo01", "philo02", "philo03",
    "philo04", "philo05", "philo06", "philo07",
    "philo08", "philo09"
};

static sem_ptr forks[MAX_PHILOS] = {NULL};

static sem_ptr modifing;

int philos_pids[MAX_PHILOS];
int philos_states[MAX_PHILOS];

int16_t philo_fds[3] = {NO_INPUT, STDOUT, STDERR};

int init_philos_restaurant(uint64_t argc, char *argv[]) {
    int cant_philos = MIN_PHILOS;
    modifing = my_sem_create(MAX_PHILOS, 1);
    for (int j = 0; j < MIN_PHILOS; j++) {     
        philos_states[j] = THINKING;
    }

    char* args[3];
    for(int i=0; i < MIN_PHILOS; i++) {
        args[0] = philos[i];
        intToString(i, args[1]);    
        args[2] = 0;
        forks[i] = my_sem_create(i, 1);
        philos_pids[i] = create_process((Main)philosopher, args, philos[i] , 2, philo_fds);
    }

    return 0;
}

int philosopher(uint64_t argc, char *argv[]) {
    int philo_id = stringToInt(argv[1]);

    int left_fork_id = philo_id; 
    int right_fork_id = (philo_id + 1) % MIN_PHILOS; 

    sem_ptr* left_fork_sem;
    sem_ptr* right_fork_sem;

    if((left_fork_sem = my_sem_create(left_fork_id, 1)) == NULL || (right_fork_sem = my_sem_create(right_fork_id, 1)) == NULL) {
        printErr("\nError creating new semaphore\n");   //TO-DO: se rompe aca!!!!!!!
        return -1;
    }

    while(1) {
        if(philo_id % 2) {
            my_sem_wait(left_fork_sem);
            my_sem_wait(right_fork_sem);
        } else {
            my_sem_wait(right_fork_sem);
            my_sem_wait(left_fork_sem);
        }
        philos_states[philo_id] = EATING;
        eat(philo_id);
        my_sem_post(left_fork_sem);
        my_sem_post(right_fork_sem);
        philos_states[philo_id] = THINKING;
        sleep(THINKING_TIME);
    }
}

void eat(int philo_id) {
    for(int i=0; i < MIN_PHILOS; i++) {
        if(philos_states[i] == EATING)
            printf(" E ");
        else
            printf(" . ");
    }
    printf("\n");
}