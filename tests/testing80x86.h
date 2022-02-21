#pragma once

#include "machine.h"
#include "io.h"
#include "debug.h"
#include "testing.h"


void test_testcase(Tester *tester, char *bin_path, char *res_path, bool check_flags) {
    Machine machine = machine_create();
    CPU *cpu = machine.cpu;
    Memory *memory = machine.memory;

    Binary binary = read_binary(bin_path);
    Binary expected_binary = read_binary(res_path);

    for (size_t i = 0; i < binary.size; ++i) {
        memory->ram[0xF0000 + i] = binary.data[i];
    }

    while (!cpu->halted) {
        machine_tick(&machine);

        if (check_flags && cpu->opcode == 0x9C) { // pushf
            u16 flags_val = *(u16 *) &expected_binary.data[cpu_sp(cpu)];
            Flags expected;
            expected.word = flags_val;
            if (cpu->flags.word != expected.word) {
                char *expected_flags = flags_to_str(expected);
                char *found_flags = flags_to_str(cpu->flags);

                printf("Expected: %s [0x%04X]\n", expected_flags, expected.word);
                printf("Found   : %s [0x%04X]\n", found_flags, cpu->flags.word);

                test(tester, false, "Flags mismatch");
                dump_cpu(&machine);
                printf("---\n");
                //dump_ram(&cpu, &memory);
            }
        }
    }

    int error_count = 0;
    for (size_t i = 0; i < expected_binary.size; ++i) {
        u8 value = memory->ram[i];
        u8 expected = expected_binary.data[i];
        if (value != expected) {
            error_count++;
        }
    }

    if (error_count > 0) {
        for (size_t i = 0; i < expected_binary.size; i += 16) {
            printf("$%04lx \x1b[0;32m", i);

            for (int j = 0; j < 16; ++j) {
                printf("%02x ", expected_binary.data[i + j] & 0xFF);
            }
            printf("\x1b[0m\n");

            printf("$%04lx \x1b[0;32m", i);
            for (int j = 0; j < 16; ++j) {
                u8 a = expected_binary.data[i + j];
                u8 b = memory->ram[i + j];
                if (a != b) {
                    printf("\x1b[0;31m");
                } else {
                    printf("\x1b[0;32m");

                }
                printf("%02x ", b & 0xFF);
            }

            printf("\x1b[0m\n\n");
        }
    }
    testi(tester, error_count, 0, "Comparison with expected result");
    printf("\x1b[0;32mInstruction count: %llu\x1b[0m\n", machine.cpu->instruction_count);
}
