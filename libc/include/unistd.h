//
// Created by Grayson on 9/26/2023.
//

#ifndef UNISTD_H
#define UNISTD_H

#include <stdlib.h>

typedef int pid_t;
//typedef unsigned int intptr_t;

int write(uint32_t fd, char* buf);

pid_t fork();
pid_t getpid();
int execv(const char *filename, char const *argv[]);
int execve(const char *filename, char const *argv[], char const *envp[]);
int execvp(const char *filename, char const *argv[]);

#endif //UNISTD_H
