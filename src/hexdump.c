#include <stdio.h>
#include "io.h"


int main() {
    //Binary binary = read_binary("tests/bins/add_word.bin");
    Binary binary = read_binary("tests/bins/res_rotate.bin");

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
