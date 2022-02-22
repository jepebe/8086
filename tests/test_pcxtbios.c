#include <stdlib.h>
#include "testing.h"
#include "machine.h"
#include "io.h"

int main() {
    Tester tester = create_tester("Testcase PC XT BIOS");

    test_section("PC XT BIOS");

    Machine machine = machine_create();

    Binary binary = read_binary("../tests/bins/pcxtbios.bin");
    for (size_t i = 0; i < binary.size; ++i) {
        machine.memory->ram[0xFE000 + i] = binary.data[i];
    }

    while (!machine.cpu->halted) {
        machine_tick(&machine);
    }
    test_summary(&tester);

    printf("\x1b[0;32mInstruction count: %llu\x1b[0m\n", machine.cpu->instruction_count);

    exit(tester.fail);
}
