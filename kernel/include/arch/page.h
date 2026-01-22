#ifndef GOOSE_PAGE_H
#define GOOSE_PAGE_H

#include <stdlib.h>

#define PAGE_PRESENT         0b0001
#define PAGE_WRITABLE        0b0010
#define PAGE_USER_ACCESS     0b0100

inline void invalidate_page(uint32_t virtual_address)
{
    __asm __volatile__ ("invlpg %0" :: "m"(virtual_address));
}

#endif //GOOSE_PAGE_H