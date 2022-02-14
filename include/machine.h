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

/// Return an address with segment override included or default segment
u32 effective_addr(Machine *m, u16 offset, SegmentOverride default_segment);
u32 effective_addr_no_override(Machine *m, u16 offset, SegmentOverride segment);

void machine_tick(Machine *machine);
