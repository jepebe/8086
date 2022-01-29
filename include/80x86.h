#pragma once

#define static_assert _Static_assert


//#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <time.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define s8 int8_t
#define s16 int16_t
#define s32 int32_t
#define s64 int64_t

//#define LEN(x) (sizeof(x) / sizeof((x)[0]))
//
//
//u8 count_set_bits(u64 n) {
//    int count = 0;
//    while (n) {
//        n = n & (n - 1);
//        count++;
//    }
//    return count;
//}
//
//void print_bits(size_t bits, u64 value) {
//    for (int i = bits - 1; i >= 0; i--) {
//        unsigned char byte = (value >> i) & 1;
//        printf("%d", byte);
//    }
//    puts("");
//}
//
//
//u64 min_u64(u64 a, u64 b) {
//    return a <= b ? a : b;
//}
