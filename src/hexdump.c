#include <stdio.h>
#include <string.h>
#include "io.h"
#include "80x86.h"


char *basename(char *path, char *default_basename) {
    char *name = strrchr(path, '/');

    // check for at least one char in string
    if (!name && !(name + 1)) {
        return default_basename;
    }
    return name + 1;
}

void usage(char *program) {
    char *prog_name = basename(program, "basename");
    printf("Usage: %s <binary_file>\n", prog_name);
}

int main(int argc, char *argv[argc]) {

    if (argc < 2) {
        usage(argv[0]);
        exit(1);
    }
    Binary binary = read_binary(argv[1]);
    printf("Path : %s\n", binary.path);
    printf("Size : %lu bytes\n", binary.size);

    size_t segments = binary.size / 16;
    // add segment if size is not divisible by 16
    segments += (binary.size % 16) > 0 ?  1 : 0;

    for (size_t segment = 0; segment < segments; ++segment) {
        printf("$%04lX  ", segment * 16);

        size_t row_start = segment * 16;
        size_t row_end = row_start + 16;
        for (size_t row_index = row_start; row_index < row_end; ++row_index) {
            if(row_index < binary.size) {
                printf("%02X ", binary.data[row_index] & 0xFF);
            } else {
                printf("   ");
            }
        }

        printf(" ");

        for (size_t row_index = row_start; row_index < row_end; ++row_index) {
            if(row_index < binary.size) {
                u8 c = binary.data[row_index];
                if (c <= 0x20) {
                    printf("\xe2\x90%c", c + 0x80);
                } else {
                    printf("%c", c);
                }
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}
