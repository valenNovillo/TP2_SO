#include <stdint.h>
#include <sys/types.h>
#include "../Drivers/include/keyBoardDriver.h"
#include "../Drivers/include/audioDriver.h"
#include "../Drivers/include/videoDriver.h"
#include "../Drivers/include/timeDriver.h"
#include "../Drivers/include/fileDescriptors.h"
#include "../include/processes.h"
#include "../include/scheduler.h"
#include "../include/semaphore.h"

//Devuelve la cantidad de caracteres que pudo leer y -1 si hubo un error
static ssize_t sys_call_read(uint64_t fd, char * buff, uint64_t count, uint64_t r10, uint64_t r8) {
    if(fd != STDIN)
        return -1;
    return fillBuf(buff, count);
}

//Devuelve la cantidad de caracteres que pudo escribir
static ssize_t sys_call_write(uint64_t fd, char* buff, uint64_t count, uint64_t r10, uint64_t r8) { 
    if (fd != STDOUT && fd != STDERR) {
        return -1;
    }
    return putString(fd, buff, count);
}

//Genera un beep
static ssize_t sys_call_beep(uint64_t freq , uint64_t duration) {
    beep(freq, duration);
    return 0;
}

static ssize_t sys_call_seconds_elapsed()
{
    return seconds_elapsed();
}

static ssize_t sys_call_incSize() {
    sizeInc();
    return 0;
}

static ssize_t sys_call_decSize() {
    sizeDec();
    return 0;
}

static ssize_t sys_call_seconds() { 
    return bringSeconds();
}

static ssize_t sys_call_minutes() {
    return bringMinutes();
}

static ssize_t sys_call_hours() {
    return bringHours();
}

static ssize_t sys_call_cleanScreen() {
    cleanScreen();
    return 0;
}

static ssize_t sys_call_sleep(unsigned int ms) {
    wait(ms);
    return 0;
}

static ssize_t sys_call_setColor(unsigned char b, unsigned char g, unsigned char r) {
    Color color = {b, g, r};
    setColor(color);
    return 0;
}

static ssize_t sys_call_setSize(unsigned int size) {
    setSize(size);
    return 0;
}

static ssize_t sys_call_printSquare(unsigned char color[3], int x, int y, int length) {
    Color hexColor = {color[0], color[1], color[2]};
    printSquare(hexColor, x, y, length);
    return 0;
}

static ssize_t sys_call_reading(unsigned char flag) {
    return setReading(flag);
}

static ssize_t sys_call_create_process(Main process_main, char** args, char* name, uint8_t priority, int16_t fds[]) {
    return create_process(process_main, args, name, priority, fds);
}

static ssize_t sys_call_get_pid(){
    return get_pid();
}

static ssize_t sys_call_kill_process(uint16_t pid) {
    return kill_process(pid, -1);
}

static ssize_t sys_call_set_priority(uint16_t pid, uint8_t new_p) {
    return set_priority(pid, new_p);
}

static ssize_t sys_call_block_process(uint16_t pid) {
    return block_process(pid);
}

static ssize_t sys_call_unblock_process(uint16_t pid) {
    return unblock_process(pid);
}

static ssize_t sys_call_yield() {
    yield();
    return 0;
}

static ssize_t sys_call_wait_pid(int16_t pid) {
    return wait_pid(pid);
}

static ssize_t sys_call_ps(){ 
    ps();
    return 0;
}

static ssize_t sys_call_hlt() {
    _hlt();
    return 0;
}

static ssize_t sys_call_sem_create(uint8_t id, uint64_t value) {
    return my_sem_create(id, value);
}

static ssize_t sys_call_sem_destroy(semaphore* semaphore_ptr) {
    my_sem_destroy(semaphore_ptr);
    return 0;
}

static ssize_t sys_call_sem_open(uint8_t id){
    return my_sem_open(id);
}

static ssize_t sys_call_sem_post(semaphore* semaphore_ptr){
    return my_sem_post(semaphore_ptr);
}

static ssize_t sys_call_sem_wait(semaphore* semaphore_ptr) {
    return my_sem_wait(semaphore_ptr);
}



static ssize_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {
    sys_call_read, 
    sys_call_write, 
    sys_call_beep, 
    sys_call_seconds_elapsed,
    sys_call_incSize,
    sys_call_decSize, 
    sys_call_seconds,
    sys_call_minutes, 
    sys_call_hours, 
    sys_call_cleanScreen, 
    sys_call_sleep, 
    sys_call_setColor, 
    sys_call_setSize,
    sys_call_printSquare,
    sys_call_reading,
    sys_call_create_process,
    sys_call_get_pid,
    sys_call_kill_process,
    sys_call_set_priority,
    sys_call_block_process,
    sys_call_unblock_process,
    sys_call_yield,
    sys_call_wait_pid,
    sys_call_ps,
    sys_call_hlt,
    sys_call_sem_create,
    sys_call_sem_destroy,
    sys_call_sem_open,
    sys_call_sem_post,
    sys_call_sem_wait
    };


ssize_t sysCallDispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) {
    if (rax >= sizeof(syscall_handlers) / sizeof(syscall_handlers[0])) 
            return -1;

    return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
}