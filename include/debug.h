#pragma once

#include "80x86.h"
#include "machine.h"


char *flags_to_str(Flags flags);

NO_RETURN void cpu_error(Machine *machine, char *message, ...);

void cpu_note(Machine *machine, char *format, ...);

void cpu_error_marker(Machine *machine, char *file, int line);

void cpu_instruction_context(Machine *machine);

void cpu_peek(Machine *machine);

void memory_peek(Machine *machine, u16 segment);

void dump_cpu(Machine *machine);

void dump_ram(Machine *machine);

void print_stack(Machine *machine);

void print_flags(Machine *machine);

void print_cpu(Machine *machine);
