#include <stdlib.h>
#include <stdio.h>
#include "debug.h"

void dump_ram(Machine *machine) {
    for (int i = 0; i < 0x100; i += 16) {
        printf("$%04X ", i);
        for (int j = 0; j < 16; ++j) {
            printf("%02x ", machine->memory->ram[i + j] & 0xFF);
        }
        printf("\n");
    }
}

void dump_cpu(Machine *machine) {
    CPU *cpu = machine->cpu;
    printf("\x1b[0;33m");
    printf("[SS:SP] = $%04X:%04X ", cpu->SS, cpu->SP);
    printf("[CS:IP] = $%04X:%04X ", cpu->CS, cpu->IP);
    printf("[DS:SI] = $%04X:%04X ", cpu->DS, cpu->SI);
    printf("[ES:DI] = $%04X:%04X\n", cpu->ES, cpu->DI);
    printf("[AX] = 0x%04X ", cpu->AX);
    printf("[BX] = 0x%04X ", cpu->BX);
    printf("[CX] = 0x%04X ", cpu->CX);
    printf("[DX] = 0x%04X ", cpu->DX);
    printf("\x1b[0m\n");
}

void cpu_instruction_context(Machine *machine) {
    u32 segment = cpu_ip(machine->cpu) & 0xFFFF0;
    if (segment > 0x10) {
        segment -= 0x10;
    }
    for(int seg = 0; seg < 3; ++seg) {
        printf("\x1b[0;31m[$%05X] ", segment);
        for (int i = 0; i < 16; ++i) {
            u32 addr = segment + i;
            printf("%02X ", machine->memory->ram[addr]);
        }
        printf("\x1b[0m\n");
        segment += 0x10;
    }
}

NO_RETURN void cpu_error(Machine *machine, char *message, int value) {
    printf("\x1b[0;31m");
    printf("[$%05X] Error: ", cpu_ip(machine->cpu));
    printf(message, value);
    printf("\x1b[0m\n");
    cpu_instruction_context(machine);

    exit(1);
}

void cpu_note_int(Machine *machine, char *message, int value) {
    printf("\x1b[0;33m");
    printf("[$%05X] Note: ", cpu_ip(machine->cpu));
    printf(message, value);
    printf("\x1b[0m\n");
}

void cpu_error_marker(Machine *machine, char *file, int line) {
    printf("\x1b[0;33m");
    printf("[$%05X] Note: %s:%d", cpu_ip(machine->cpu), file, line);
    printf("\x1b[0m\n");
}

void cpu_note_str(Machine *machine, char *message, char *value) {
    printf("\x1b[0;31m");
    printf("[$%05X] Error: ", cpu_ip(machine->cpu));
    printf(message, value);
    printf("\x1b[0m\n");
}

void cpu_peek(Machine *machine) {
    u32 segment = cpu_ip(machine->cpu) & 0xFFFF0;
    printf("\x1b[0;31m[$%05X] ", segment);
    for (int i = 0; i < 16; ++i) {
        u32 addr = segment + i;
        printf("%02X ", machine->memory->ram[addr]);
    }
    printf("\x1b[0m\n");
}

char *flags_to_str(Flags flags) {
    char *res = malloc(sizeof(char) * 10);
    res[9] = '\0';
    res[8] = flags.CF ? 'c' : '.';
    res[7] = flags.PF ? 'p' : '.';
    res[6] = flags.AF ? 'a' : '.';
    res[5] = flags.ZF ? 'z' : '.';
    res[4] = flags.SF ? 's' : '.';
    res[3] = flags.TF ? 't' : '.';
    res[2] = flags.IF ? 'i' : '.';
    res[1] = flags.DF ? 'd' : '.';
    res[0] = flags.OF ? 'o' : '.';
    return res;
}

NO_RETURN void cpu_error_int(Machine *machine, char *message, int value) {
    printf("\x1b[0;31m");
    printf("[$%05X] Error: ", cpu_ip(machine->cpu));
    printf(message, value);
    printf("\n");
    cpu_instruction_context(machine);
    exit(1);
}

NO_RETURN void cpu_error_str(Machine *machine, char *message, char *value) {
    printf("\x1b[0;31m");
    printf("[$%05X] Error: ", cpu_ip(machine->cpu));
    printf(message, value);
    printf("\n");
    cpu_instruction_context(machine);
    exit(1);
}

void memory_peek(Machine *machine, u16 segment) {
    u32 seg = segment << 4;
    printf("\x1b[0;31m[$%05X] ", seg);
    for (int i = 0; i < 16; ++i) {
        u32 addr = seg + i;
        printf("%02X ", machine->memory->ram[addr]);
    }
    printf("\x1b[0m\n");
}

void print_stack(Machine *machine) {
    u32 addr = cpu_sp(machine->cpu);
    printf("\x1b[0;33m[SS:SP=$%04X:%04X] ", machine->cpu->SS, machine->cpu->SP);
    printf("[ ");
    for(int i = 0; i < 5; ++i) {
        printf("0x%04X ", read_memory_u16(addr + i * 2, machine->memory));
    }
    printf("]\x1b[0m\n");
}

void print_flags(Machine *machine) {
    char *flags = flags_to_str(machine->cpu->flags);
    printf("\x1b[0;33m[flags=0x%04X ", machine->cpu->flags.word);
    printf("%s", flags);
    printf("]\x1b[0m\n");
}
