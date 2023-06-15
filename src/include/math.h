//
// Created by grsan on 6/6/23.
//

#ifndef GOOSE_MATH_H
#define GOOSE_MATH_H

#include <stdint.h>

uint32_t flp2 (uint32_t x)
{
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x - (x >> 1);
}

#endif //GOOSE_MATH_H
