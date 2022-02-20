#pragma once

#include "80x86.h"
#include "machine.h"
#include "debug.h"

typedef enum {
    AL = 0x0,
    CL = 0x1,
    DL = 0x2,
    BL = 0x3,
    AH = 0x4,
    CH = 0x5,
    DH = 0x6,
    BH = 0x7,
    AX = 0x8,
    CX = 0x9,
    DX = 0xA,
    BX = 0xB,
    SP = 0xC,
    BP = 0xD,
    SI = 0xE,
    DI = 0xF,
} REG;

typedef enum {
    ES = 0x0,
    CS = 0x1,
    SS = 0x2,
    DS = 0x3,
} SegREG;

// Merged table 1 og table 2
// Only difference was DIRECT / BP_PTR
typedef enum {
    /// [BX+SI]
    BX_SI_PTR = 0x00,
    BX_DI_PTR = 0x01,
    BP_SI_PTR = 0x02,
    BP_DI_PTR = 0x03,
    SI_PTR = 0x04,
    DI_PTR = 0x05,
    DIRECT_OR_BP_PTR = 0x06,
    BX_PTR = 0x07
} MemoryModeTable;

typedef enum {
    NO_DISP,
    BYTE_DISP,
    WORD_DISP,
} DisplacementType;

typedef struct {
    MemoryModeTable memory_mode;
    DisplacementType displacement_type;
    OperandSize operand_size;
} MemoryMode;

void fetch_addressing_mode(Machine *machine);

AddrMode peek_addressing_mode(Machine *machine);

void decode_register(Machine *machine, REG reg, Operand *op);

void decode_segment_register(Machine *machine, SegREG reg, Operand *op);

void decode_operand(Machine *machine, MemoryMode mode, Operand *op);

//Operand get_write_memory(Machine *machine, MemoryMode mode);
