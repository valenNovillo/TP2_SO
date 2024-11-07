#include "./include/applications.h"
#include "../Library/include/stdio1.h"
#include "../Library/include/unistd1.h"
#define SLEEP_TIME 5000


void loop() {
    int pid = get_pid(); 

    while (1) {
        printf("\nHi! I'm process %d and I'll be looping every %d seconds\n", pid, SLEEP_TIME/1000);
        sleep(SLEEP_TIME);  
    }
}