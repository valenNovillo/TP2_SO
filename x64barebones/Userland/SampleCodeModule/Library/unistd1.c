#include "include/unistd1.h"

extern ssize_t sysCall(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8);

int read(int fd, char * buff, int count) {
    return sysCall(0, fd, buff, count, 0, 0);
}

ssize_t write(int fildes, const void *buf, size_t nbyte) {
    return sysCall(1, fildes, buf, nbyte, 0, 0);
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
    sysCall(13, color, x, y, length, 0);
}

int reading(unsigned char flag) {
    return sysCall(14, flag, 0, 0, 0, 0);
}

uint16_t create_process(Main process_main, char** args, char* name, uint8_t priority, int16_t fds[]) {
    return sysCall(15, process_main, args, name, priority, fds);
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

int block_process(uint16_t pid){
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

