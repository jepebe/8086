#pragma once

#include <stdbool.h>
#include "80x86.h"
#include "memory.h"
#include "cpu.h"

typedef struct {
    CPU *cpu;
    Memory *memory;
} Machine;

Machine machine_create();

void machine_tick(Machine *machine);
