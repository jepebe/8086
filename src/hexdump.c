#include <stdio.h>
#include <string.h>
#include "io.h"

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
    
    for (size_t i = 0; i < binary.size; ++i) {
        if ((i % 16) == 0) {
            printf("%04lx ", i);
        }

        printf("%02x ", binary.data[i] & 0xFF);

        if (((i + 1) % 16) == 0) {
            printf("\n");
        }
    }
    return 0;
}
