#include "include/unistd1.h"

extern ssize_t sysCall(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8);

int read(int fd, char * buff, int count) {
    return sysCall(0, fd, (uint64_t)buff, count, 0, 0);
}

ssize_t write(int fildes, const void *buf, size_t nbyte) {
    return sysCall(1, fildes, (uint64_t)buf, nbyte, 0, 0);
}

void beep(uint64_t freq , uint64_t duration) {
    sysCall(2,freq, duration, 0, 0, 0);
}

int getSecondsElapsed() {
   return sysCall(3, 0, 0, 0, 0, 0);  
}

void sizeInc() {
    sysCall(4, 0, 0, 0, 0, 0);
}

void sizeDec() {
    sysCall(5, 0, 0, 0, 0, 0);
}

int getSeconds() {
    return sysCall(6, 0, 0, 0, 0, 0);
}

int getMinutes() {
    return sysCall(7, 0, 0, 0, 0, 0);
}

int getHours() {
    return sysCall(8, 0, 0, 0, 0, 0);
}

void cleanScreen() {
    sysCall(9, 0, 0, 0, 0, 0);
}

void sleep(unsigned int ms) {
    sysCall(10, ms, 0, 0, 0, 0);
}

void setColor(unsigned char b, unsigned char g, unsigned char r) {
    sysCall(11, b, g, r, 0, 0);
}

void setSize(unsigned int size) {
    sysCall(12, size, 0, 0, 0, 0);
}

void printSquare(unsigned char color[3], int x, int y, int length) {
    sysCall(13, (uint64_t)color, x, y, length, 0);
}

int reading(unsigned char flag) {
    return sysCall(14, flag, 0, 0, 0, 0);
}

uint16_t create_process(Main process_main, char** args, char* name, uint8_t priority, int16_t fds[]) {
    return sysCall(15, (uint64_t)process_main, (uint64_t)args, (uint64_t)name, priority, (uint64_t)fds);
}

uint16_t get_pid(){
    return sysCall(16, 0, 0, 0, 0, 0);
}

int32_t kill_process(uint16_t pid) {
    return sysCall(17, pid, 0, 0, 0, 0);
}

int32_t set_priority(uint16_t pid, uint8_t new_p) {
    return sysCall(18, pid, new_p, 0, 0, 0);
}

int block_process(uint16_t pid) {
    return sysCall(19,pid, 0, 0, 0, 0);
}

int unblock_process(uint16_t pid) {
    return sysCall(20, pid, 0, 0, 0, 0);
}

void yield(){
   sysCall(21,0, 0, 0, 0, 0); 
}

uint64_t wait_pid(int16_t pid) {
    return sysCall(22, pid, 0, 0, 0, 0);
}

int ps(){
    return sysCall(23, 0, 0, 0, 0, 0);
}

void _hlt(){
    sysCall(24, 0, 0, 0, 0, 0);
}

sem_ptr my_sem_create(uint8_t id, uint64_t value) {
    return (sem_ptr)sysCall(25, id, value, 0, 0, 0);
}

void my_sem_destroy(sem_ptr ptr){
    sysCall(26, (uint64_t)ptr, 0, 0, 0, 0);
}

sem_ptr my_sem_open(uint8_t id) {
    return (sem_ptr)sysCall(27, id, 0, 0, 0, 0);
}


uint8_t my_sem_post(sem_ptr ptr){
    return sysCall(28, (uint64_t)ptr, 0, 0, 0, 0);
}


uint8_t my_sem_wait(sem_ptr ptr) {
    return sysCall(29, (uint64_t)ptr, 0, 0, 0, 0);
}

int print_mem_status() {
    return sysCall(30, 0, 0, 0, 0, 0);
}

void* my_malloc(uint64_t size) {
    return (void*)sysCall(31, size, 0, 0, 0, 0);
}

void my_free(void* ptr) {
    sysCall(32, (uint64_t)ptr, 0, 0, 0, 0);
}

uint16_t get_state(uint16_t pid) {
    return sysCall(33, pid, 0, 0, 0, 0);
}

int16_t open_pipe_for_pid(int16_t id, int16_t pid, char mode) {
    return sysCall(34, id, pid, mode, 0, 0);
}

int16_t open_pipe(int16_t id, char mode) {
    return sysCall(35, id, mode, 0, 0, 0);
}

void close_pipe_for_pid(int16_t id, int16_t pid) {
    sysCall(36, id, pid, 0, 0, 0);
}

int16_t* get_fds() {
    return (int16_t*)sysCall(37, 0, 0, 0, 0, 0);
}
