#pragma once

#include "machine.h"
#include "opcodes.h"

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

void disassemble_instruction(Machine *machine, u32 addr, u8 opc, Opcode opcode, Operand *out, Operand *in);
