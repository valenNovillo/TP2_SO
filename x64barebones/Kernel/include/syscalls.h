#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <sys/types.h>

typedef ssize_t (*Syscall)(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8);

#endif