#include <stdint.h>
#include <stddef.h>
//#include <syscall.h>
#include "test_util.h"
#include "../Library/include/unistd1.h"
#include "../Library/include/stdio1.h"

//#define SEM_ID "sem"
#define SEM_ID 5
#define TOTAL_PAIR_PROCESSES 5

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  int64_t aux = *p;
  yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  sem_ptr sem = NULL;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  if (use_sem) {
    /*if (!sem_open(SEM_ID, 1)) {
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }*/
   sem = my_sem_open(SEM_ID);
   if (sem == NULL) {
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }
  }
  uint64_t i;
  for (i = 0; i < n; i++) {
    /*if (use_sem)
      //my_sem_wait(SEM_ID);*/
      if(use_sem){
        my_sem_wait(sem);
      }
    slowInc(&global, inc);
    /*if (use_sem)
      my_sem_post(SEM_ID);*/
      if(use_sem){
        my_sem_post(sem);
      }
      
  }

  /*if (use_sem)
    my_sem_close(SEM_ID); */

  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  int16_t fds[] = {NO_INPUT, STDOUT, STDERR};
  sem_ptr sem = my_sem_create(SEM_ID, 1);

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    //pids[i] = my_create_process("my_process_inc", 3, argvDec);
    //pids[i + TOTAL_PAIR_PROCESSES] = my_create_process("my_process_inc", 3, argvInc);
    pids[i] = create_process((Main)my_process_inc, argvDec, "my_process_dec", 3, fds);
    pids[i + TOTAL_PAIR_PROCESSES] = create_process((Main)my_process_inc, argvInc, "my_process_inc", 3, fds);
    
    
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    wait_pid(pids[i]);
    wait_pid(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  my_sem_destroy(sem);

  printf("Final value: %d\n", global);

  return 0;
}