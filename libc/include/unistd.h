//
// Created by Grayson on 9/26/2023.
//

#ifndef UNISTD_H
#define UNISTD_H

typedef int pid_t;
typedef unsigned int intptr_t;

pid_t fork();
pid_t getpid();
int execv(const char *filename, char const *argv[]);
int execve(const char *filename, char const *argv[], char const *envp[]);
int execvp(const char *filename, char const *argv[]);

#endif //UNISTD_H
