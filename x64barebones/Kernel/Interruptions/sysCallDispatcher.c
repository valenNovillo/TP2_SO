#include "../include/syscalls.h"
#include "../Drivers/include/keyBoardDriver.h"
#include "../Drivers/include/audioDriver.h"
#include "../Drivers/include/videoDriver.h"
#include "../Drivers/include/timeDriver.h"
#include "../Drivers/include/fileDescriptors.h"
#include "../include/processes.h"
#include "../include/scheduler.h"
#include "../include/semaphore.h"
#include "../include/memoryManager.h"
#include "../include/pipe.h"

//Devuelve la cantidad de caracteres que pudo leer y -1 si hubo un error
static ssize_t sys_call_read(uint64_t fd, char * buff, uint64_t count, uint64_t r10, uint64_t r8){
    if(fd == STDIN){
        return fillBuf(buff, count);
    }else if(fd > STDIN){
        return read_on_file(fd, buff, count);
    }

    return -1;
}

//Devuelve la cantidad de caracteres que pudo escribir
static ssize_t sys_call_write(uint64_t fd, char* buff, uint64_t count, uint64_t r10, uint64_t r8) { 
    if (fd == STDOUT || fd == STDERR) {
        putString(fd, buff, count);
    }else if(fd > STDERR){
        return write_on_file(fd, buff, count);
    }
    return -1;
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
    return (ssize_t) my_sem_create(id, value);
}

static ssize_t sys_call_sem_destroy(semaphore* semaphore_ptr) {
    my_sem_destroy(semaphore_ptr);
    return 0;
}

static ssize_t sys_call_sem_open(uint8_t id){
    return (ssize_t) my_sem_open(id);
}

static ssize_t sys_call_sem_post(semaphore* semaphore_ptr){
    return (ssize_t) my_sem_post(semaphore_ptr);
}

static ssize_t sys_call_sem_wait(semaphore* semaphore_ptr) {
    return (ssize_t) my_sem_wait(semaphore_ptr);
}

static ssize_t sys_call_print_mem_status(){
    print_mem_status();
    return 0;
}

static ssize_t sys_call_my_malloc(uint64_t size){
    return (ssize_t) my_malloc(size);
}

static ssize_t sys_call_my_free(void* ptr){
    my_free(ptr);
    return 0;
}

static ssize_t sys_call_get_state(uint16_t pid){
    return get_state(pid);
}

static ssize_t sys_call_open_pipe_for_pid(int16_t id, int16_t pid, char mode){
    return open_pipe_for_pid(id, pid, mode);
}

static ssize_t sys_call_open_pipe(int16_t id, char mode){
    return open_pipe(id, mode);
}

static ssize_t sys_call_close_pipe_for_pid(int16_t id, int16_t pid){
    return open_pipe(id, pid);
}

static Syscall syscall_handlers[] = {
    (Syscall) sys_call_read, 
    (Syscall) sys_call_write, 
    (Syscall) sys_call_beep, 
    (Syscall) sys_call_seconds_elapsed,
    (Syscall) sys_call_incSize,
    (Syscall) sys_call_decSize, 
    (Syscall) sys_call_seconds,
    (Syscall) sys_call_minutes, 
    (Syscall) sys_call_hours, 
    (Syscall) sys_call_cleanScreen, 
    (Syscall) sys_call_sleep, 
    (Syscall) sys_call_setColor, 
    (Syscall) sys_call_setSize,
    (Syscall) sys_call_printSquare,
    (Syscall) sys_call_reading,
    (Syscall) sys_call_create_process,
    (Syscall) sys_call_get_pid,
    (Syscall) sys_call_kill_process,
    (Syscall) sys_call_set_priority,
    (Syscall) sys_call_block_process,
    (Syscall) sys_call_unblock_process,
    (Syscall) sys_call_yield,
    (Syscall) sys_call_wait_pid,
    (Syscall) sys_call_ps,
    (Syscall) sys_call_hlt,
    (Syscall) sys_call_sem_create,
    (Syscall) sys_call_sem_destroy,
    (Syscall) sys_call_sem_open,
    (Syscall) sys_call_sem_post,
    (Syscall) sys_call_sem_wait,
    (Syscall) sys_call_print_mem_status,
    (Syscall) sys_call_my_malloc,
    (Syscall) sys_call_my_free,
    (Syscall) sys_call_get_state,
    (Syscall) sys_call_open_pipe_for_pid,
    (Syscall) sys_call_open_pipe,
    (Syscall) sys_call_close_pipe_for_pid
    };


ssize_t sysCallDispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) {
    if (rax >= sizeof(syscall_handlers) / sizeof(syscall_handlers[0])) 
            return -1;

    return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
}