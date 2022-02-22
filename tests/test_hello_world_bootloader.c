#include <stdlib.h>
#include "testing.h"
#include "machine.h"
#include "io.h"

int main() {
    Tester tester = create_tester("Testcase Hello World Bootloader");

    test_section("Hello World Bootloader");

    Machine machine = machine_create();

    Binary binary = read_binary("../tests/bins/hello_world_bootloader.bin");
    for (size_t i = 0; i < binary.size; ++i) {
        machine.memory->ram[0x07C00 + i] = binary.data[i];
    }

    machine.memory->ram[0xFFFF0] = 0xEA;
    machine.memory->ram[0xFFFF1] = 0x00;
    machine.memory->ram[0xFFFF2] = 0x00;
    machine.memory->ram[0xFFFF3] = 0xC0;
    machine.memory->ram[0xFFFF4] = 0x07;

    while (!machine.cpu->halted) {
        machine_tick(&machine);
    }
    test_summary(&tester);

    exit(tester.fail);
}
