#pragma once

#include "80x86.h"
#include "machine.h"

typedef enum {
    MMT_DEFAULT = 0,
    MMT_POINTER = 1,
    MMT_SEGMENT_OVERRIDABLE = 2,
    MMT_ADDRESS = 4,
    MMT_VALUE_BYTE = 8,
    MMT_VALUE_WORD = 16,
    MMT_DISPLACEMENT_BYTE = 32,
    MMT_DISPLACEMENT_WORD = 64,
    MMT_ADDRESS_32BIT = 128,
    MMT_EMPTY = 256,
} MemoryMnemonicType;

typedef struct {
    MemoryMnemonicType type;
    const char *name;
} MemoryMnemonic;

char *flags_to_str(Flags flags);

NO_RETURN void cpu_error(Machine *machine, char *message, int value);

void cpu_note_int(Machine *machine, char *message, int value);
void cpu_note_u32(Machine *machine, char *message, u32 value);

void cpu_note_str(Machine *machine, char *message, char *value);

void cpu_error_marker(Machine *machine, char *file, int line);

void cpu_peek(Machine *machine);

void memory_peek(Machine *machine, u16 segment);

NO_RETURN void cpu_error_int(Machine *machine, char *message, int value);

NO_RETURN void cpu_error_str(Machine *machine, char *message, char *value);

void dump_cpu(Machine *machine);

void dump_ram(Machine *machine);

void cpu_instruction_context(Machine *machine);

void print_stack(Machine *machine);

void print_flags(Machine *machine);

void print_cpu(Machine *machine);
