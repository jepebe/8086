#pragma once

#include "80x86.h"
#include "machine.h"

char *flags_to_str(Flags flags);

NO_RETURN void cpu_error(Machine *machine, char *message, int value);

void cpu_note_int(Machine *machine, char *message, int value);

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
