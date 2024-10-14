#include <stdint.h>
#include <sys/types.h>
#include "../Drivers/include/keyBoardDriver.h"
#include "../Drivers/include/audioDriver.h"
#include "../Drivers/include/videoDriver.h"
#include "../Drivers/include/timeDriver.h"
#include "../Drivers/include/fileDescriptors.h"
#include "../include/processes.h"
#include "../include/scheduler.h"

//Devuelve la cantidad de caracteres que pudo leer y -1 si hubo un error
static ssize_t sys_call_read(uint64_t fd, uint64_t buff, uint64_t count, uint64_t r10, uint64_t r8) {
    if(fd != STDIN)
        return -1;
    return fillBuf((char*)buff, count);
}

//Devuelve la cantidad de caracteres que pudo escribir
static ssize_t sys_call_write(uint64_t fd, uint64_t buff, uint64_t count, uint64_t r10, uint64_t r8) { 
    if (fd != STDOUT && fd != STDERR) {
        return -1;
    }
    return putString(fd, (char*)buff, count);
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

static ssize_t sys_call_sleep(uint64_t ms) {
    wait((unsigned int)ms);
    return 0;
}

static ssize_t sys_call_setColor(uint64_t b, uint64_t g, uint64_t r) {
    Color color = {(unsigned char)b, (unsigned char)g,(unsigned char)r};
    setColor(color);
    return 0;
}

static ssize_t sys_call_setSize(uint64_t size) {
    setSize((unsigned int)size);
    return 0;
}

static ssize_t sys_call_printSquare(unsigned char color[3], uint64_t x, uint64_t y, uint64_t length) {
    Color hexColor = {color[0], color[1], color[2]};
    printSquare(hexColor, (int)x, (int)y, (int)length);
    return 0;
}

static ssize_t sys_call_reading(uint64_t flag) {
    return setReading((unsigned char)flag);
}

static ssize_t sys_call_create_process(uint64_t process_main, uint64_t args, uint64_t name, uint64_t priority, uint64_t fds[]) {
    return create_process((Main)process_main, (char**)args, (char*)name, (uint8_t)priority, (int16_t)fds);
}

static ssize_t sys_call_get_pid(){
    return get_pid();
}

static ssize_t sys_call_kill_process(uint64_t pid) {
    return kill_process((uint16_t)pid, -1);
}

static ssize_t sys_call_set_priority(uint64_t pid, uint64_t new_p) {
    return set_priority((uint16_t)pid, (uint8_t)new_p);
}

static ssize_t sys_call_block_process(uint64_t pid) {
    return block_process((uint16_t)pid);
}

static ssize_t sys_call_unblock_process(uint64_t pid) {
    return unblock_process((uint16_t)pid);
}

static ssize_t sys_call_yield() {
    yield();
    return 0;
}

static ssize_t sys_call_wait_pid(uint64_t pid) {
    return wait_pid((int16_t)pid);
}

static ssize_t sys_call_ps(){ 
    ps();
    return 0;
}

static ssize_t sys_call_hlt() {
    _hlt();
    return 0;
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
    sys_call_hlt
    };


ssize_t sysCallDispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) {
    if (rax >= sizeof(syscall_handlers) / sizeof(syscall_handlers[0])) 
            return -1;

    return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
}