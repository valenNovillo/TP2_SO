#include <stdint.h>
#include "../Library/include/unistd1.h"
#include "../Library/include/stdio1.h"
#include "../Library/include/philosophers.h"
#include "../Test/test_util.h"
#include <stddef.h>
#define MIN_PHILOS 3
#define INITIAL_PHILOS 5
#define MAX_PHILOS 10
#define THINKING 0
#define EATING 1
#define THINKING_TIME 1000000000
#define EATING_TIME 15000

static char *philos[MAX_PHILOS] = {
    "philo00", "philo01", "philo02", "philo03",
    "philo04", "philo05", "philo06", "philo07",
    "philo08", "philo09"
};

static sem_ptr forks[MAX_PHILOS] = {NULL};
static sem_ptr modifing;
static sem_ptr printing;
static int cant_philos;

int philos_pids[MAX_PHILOS];
int philos_states[MAX_PHILOS];

int16_t philo_fds[3] = {NO_INPUT, STDOUT, STDERR};

static void destroy_all(){
    my_sem_destroy(modifing);
    my_sem_destroy(printing);
    for(int i=0; i < cant_philos; i++){
        my_sem_destroy(forks[i]);
        kill_process(philos_pids[i]);
    }
}

static void add_philo(){
    my_sem_wait(modifing);
    if (cant_philos == MAX_PHILOS){
        my_sem_wait(printing);
        printErr("\nMax quantity of philosophers reached\n");
        my_sem_post(printing);
        my_sem_post(modifing);
        return;
    }
    char* args[2];   
    char id[3];
    intToString(cant_philos, id);
    args[0] = id;
    args[1] = 0;

    if((cant_philos % 2) == 0){ 
        my_sem_wait(forks[0]);
        my_sem_wait(forks[cant_philos-1]);
        forks[cant_philos] = my_sem_create(cant_philos, 1);
        philos_states[cant_philos] = THINKING;
        philos_pids[cant_philos] = create_process((Main)philosopher, args, philos[cant_philos] , 0, philo_fds);
        my_sem_post(forks[cant_philos-1]);
        my_sem_post(forks[0]);
    } else{
        my_sem_wait(forks[cant_philos-1]);
        my_sem_wait(forks[0]);
        forks[cant_philos] = my_sem_create(cant_philos, 1);
        philos_states[cant_philos] = THINKING;
        philos_pids[cant_philos] = create_process((Main)philosopher, args, philos[cant_philos] , 0, philo_fds);
        my_sem_post(forks[0]);
        my_sem_post(forks[cant_philos-1]);
    }
    
    cant_philos++;
    
    my_sem_post(modifing);
}

static void remove_philo(){
    my_sem_wait(modifing);

    if (cant_philos == MIN_PHILOS){
        my_sem_wait(printing);
        printErr("\nMin quantity of philosophers reached\n");
        my_sem_post(printing);
        my_sem_post(modifing);
        return;
    }

    if((cant_philos % 2) == 0){
        my_sem_wait(forks[0]);
        my_sem_wait(forks[cant_philos-2]);
        my_sem_wait(forks[cant_philos-1]);
        kill_process(philos_pids[cant_philos-1]);
    } else{
        my_sem_wait(forks[cant_philos-1]);
        my_sem_wait(forks[0]);
        my_sem_wait(forks[cant_philos-2]);
        kill_process(philos_pids[cant_philos-1]);
        transer_blocked_process_by_sem(forks[cant_philos-1], forks[0]);
    }
    my_sem_destroy(forks[cant_philos-1]);
    forks[cant_philos-1] = NULL;
    my_sem_post(forks[cant_philos-2]);
    my_sem_post(forks[0]);
    cant_philos--;
    my_sem_post(modifing);
}

int init_philos_restaurant(uint64_t argc, char *argv[]){
    printf("\n");
    cant_philos = 0;
    modifing = my_sem_create(MAX_PHILOS, 1);
    printing = my_sem_create(MAX_PHILOS + 1, 1);
    
    if(modifing == NULL || printing == NULL){
        destroy_all();
        printErr("\nError creating new semaphore\n"); 
        return -1;
    }

    for (int j = 0; j < INITIAL_PHILOS; j++){     
        philos_states[j] = THINKING;
    }

    char* args[2];
    char id[3];
    for(int i=0; i < INITIAL_PHILOS; i++){
        cant_philos++;
        intToString(i, id);    
        args[0] = id;
        args[1] = 0;
        forks[i] = my_sem_create(i, 1);
        if(forks[i] == NULL){
            destroy_all();
            printErr("\nError creating new semaphore\n"); 
            return -1;
        }
        philos_pids[i] = create_process((Main)philosopher, args, philos[i] , 0, philo_fds);
    }

    int c;
    while((c = getChar()) != EOF){
        switch (c) {
            case 'a': 
                add_philo();
                break;
            case 'r':  
                remove_philo();
                break;
            default:
                my_sem_wait(printing);
                printErr("\nTecla invalida\n");
                my_sem_post(printing);
                break;
        }
    }

    destroy_all();
    
    return 0;
}

int philosopher(uint64_t argc, char *argv[]){
    int philo_id = stringToInt(argv[0]);
    while(1){
        my_sem_wait(modifing);
        if((philo_id % 2) == 0){
            my_sem_wait(forks[philo_id]);
            my_sem_wait(forks[(philo_id + 1) % cant_philos]);
        } else{
            my_sem_wait(forks[(philo_id + 1) % cant_philos]);
            my_sem_wait(forks[philo_id]);
        }
        philos_states[philo_id] = EATING;
        my_sem_post(modifing);
        eat(philo_id);
        my_sem_post(forks[philo_id]);
        my_sem_post(forks[(philo_id + 1) % cant_philos]);
        philos_states[philo_id] = THINKING;
        for (int i = 0; i < 9999 * GetUniform(THINKING_TIME) * GetUniform(THINKING_TIME); i++);
    }
}

void eat(int philo_id){
    my_sem_wait(printing);
    for(int i=0; i < cant_philos; i++){
        if(philos_states[i] == EATING)
            printf(" E ");
        else
            printf(" . ");
    }
    printf("\n");
    my_sem_post(printing);
    for (int i = 0; i < GetUniform(EATING_TIME) * GetUniform(EATING_TIME); i++);
}