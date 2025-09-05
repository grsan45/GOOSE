//
// Created by grsan on 6/6/23.
//

#ifndef MATH_H
#define MATH_H

#include <stdint.h>

// math util functions
uint32_t flp2 (uint32_t x);
uint32_t log2 (uint32_t value);
uint8_t reverse_byte(uint8_t b);

#define HASH_A 1933
#define HASH_B 6199
#define HASH(x) ((x)*(x) % (HASH_A * HASH_B))

#endif //MATH_H
