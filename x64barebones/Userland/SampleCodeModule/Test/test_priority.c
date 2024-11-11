#include <stdint.h>
#include "../Library/include/stdio1.h"
#include "../Library/include/unistd1.h"
#include "test_util.h"
#include "test.h"

#define MINOR_WAIT 1000000
#define WAIT 100000000      

/*
#define TOTAL_PROCESSES 3
#define LOWEST 0 
#define MEDIUM 1 
#define HIGHEST 2
*/

#define TOTAL_PROCESSES 6
#define LOWEST 0  
#define MEDIUM 1  
#define HIGHEST 2 
#define HIGHEST2 3
#define HIGHEST3 4
#define MAXPRIO 5


//int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};
int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST, HIGHEST2, HIGHEST3, MAXPRIO};

void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {0};
  int16_t fds[] = {NO_INPUT, STDOUT, STDERR};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    //pids[i] = my_create_process("endless_loop_print", 0, argv);
    pids[i] = create_process((Main)endless_loop, argv, "endless_loop", LOWEST, fds);

  //bussy_wait(WAIT);
  sleep(10000);
  printf("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    //my_nice(pids[i], prio[i]);
    set_priority(pids[i], prio[i]);

  //ussy_wait(WAIT);
  sleep(10000);
  printf("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    //my_block(pids[i]);
    block_process(pids[i]); 

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    //my_nice(pids[i], MEDIUM);
    set_priority(pids[i], MEDIUM);

  printf("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    //my_unblock(pids[i]);
    unblock_process(pids[i]);

  //bussy_wait(WAIT);
  sleep(10000);
  printf("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    //my_kill(pids[i]);
    kill_process(pids[i]);

  printf("FINISHED\n");
}