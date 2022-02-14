#pragma once

#define static_assert _Static_assert
#define NO_RETURN __attribute__((noreturn))


#include <stdbool.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

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
