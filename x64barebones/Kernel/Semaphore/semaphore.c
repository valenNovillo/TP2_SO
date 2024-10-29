#include <stdint.h>
#include "../include/semaphore.h"
#include "../include/memoryManager.h"
#include "../include/scheduler.h"
#include "../include/linkedList.h"

semaphore* semaphore_ptrs[MAX_SEMAPHORES];
void* start_semaphores;
int in_use[MAX_SEMAPHORES] = {0};

static uint8_t is_open(uint8_t id) {
    return in_use[id]; 
}

static int unblock_sem_process(LinkedList blocked_processes) {
    if(isEmpty(blocked_processes)) {
        return 0;
    }
    uint16_t pid = (uint16_t)dequeue(blocked_processes);
    PState state = get_state(pid);
    
    if(state != TERMINATED) {
        return unblock_process(pid);
    }

    return 0;
}

void semaphore_init(void* ptr) {
    start_semaphores = ptr;
    for(int i = 0; i < MAX_SEMAPHORES; i++) {
        semaphore_ptrs[i] = start_semaphores + sizeof(semaphore) * i;
        semaphore_ptrs[i]->id = i;
    }
}

semaphore* my_sem_create(uint8_t id, uint64_t value) {
    if (id < MAX_SEMAPHORES && !in_use[id]) {
        semaphore * aux = semaphore_ptrs[id];
        in_use[aux->id] = 1;
        aux->value = value;
        aux->blocked_proccesses = createLinkedList();
        aux->lock = 1;
        return aux;
    } else {
        return NULL;
    }
}

void my_sem_destroy(semaphore * ptr) {
    in_use[ptr->id] = 0;
    freeList(ptr->blocked_proccesses);
}



semaphore* my_sem_open(uint8_t id) {
    if(!is_open(id)) {
        return NULL;
    }

    return (semaphore*)(start_semaphores + sizeof(semaphore) * id);
}

uint8_t my_sem_post(semaphore* ptr){
    if(ptr == NULL) {
        return -1;
    }
    aquire(&(ptr->lock));
    ptr->value++;
    release(&(ptr->lock));
    return unblock_sem_process(ptr->blocked_proccesses);
}

uint8_t my_sem_wait(semaphore* ptr){
    if(ptr == NULL){
        return -1;
    }

    /*aquire(&(ptr->lock));

    if(ptr->value == 0){
        uint16_t pid = get_running_process_pid();
        Node* node = my_malloc(sizeof(Node));
        node->data = (void*) ((uint64_t) pid);
        queue(ptr->blocked_proeccesses, node);
        release(&(ptr->lock));
        set_state(pid, BLOCKED);
        aquire(&(ptr->lock));
    }*/

    
	while (1) {
		aquire(&(ptr->lock));
		if (ptr->value) {
			ptr->value--;
			release(&(ptr->lock));
			break;
		}
        uint16_t pid = get_running_process_pid();
        Node* node = my_malloc(sizeof(Node));
        node->data = (void*) ((uint64_t) pid);
        queue(ptr->blocked_proccesses, node);
        release(&(ptr->lock));
		set_state(pid, BLOCKED);
	}
    

    //ptr->value--;

    //release(&(ptr->lock));

    return 0;
}







