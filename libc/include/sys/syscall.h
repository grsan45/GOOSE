#ifndef GOOSE_SYSCALL_H
#define GOOSE_SYSCALL_H

#include <stdarg.h>
#include <stdlib.h>

#define MAX_ARGS 6 // limiting to 6 args for syscalls to simplify handler and mem management. Shouldn't ever need more

#define SYS_READ 0
#define SYS_WRITE 1
#define SYS_OPEN 2
#define SYS_CLOSE 3

uint32_t syscall(uint32_t code, uint32_t args, ...);
extern uint32_t __syscall(uint32_t code, uint32_t *args);

#endif //GOOSE_SYSCALL_H
