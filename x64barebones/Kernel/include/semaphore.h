#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include <stddef.h>

#include "linkedList.h"
 

#define MAX_SEMAPHORES 16

typedef struct semaphore {
    uint8_t id;
    uint64_t value;
    uint8_t lock;
    LinkedList blocked_proccesses;
} semaphore;


void semaphore_init(void* ptr);
semaphore* my_sem_create(uint8_t id, uint64_t value);
void my_sem_destroy(semaphore * ptr);
semaphore* my_sem_open(uint8_t id);
uint8_t my_sem_post(semaphore *ptr);
uint8_t my_sem_wait(semaphore* ptr);

extern void aquire(uint8_t * lock);
extern void release(uint8_t * lock);



#endif //SSEMAPHORE_H