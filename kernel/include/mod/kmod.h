#ifndef GOOSE_MODULES_H
#define GOOSE_MODULES_H
#include <stdlib.h>

typedef struct
{
    char *name;
    void *start;
    void *end;

    void *entry_point;
    bool loaded;
} kmod_t;



#endif //GOOSE_MODULES_H