#pragma once

#include "addressing.h"
#include "cpu.h"
#include "machine.h"
#include "debug.h"

typedef enum {
    /// Implied
    Implied,
    /// One
    ONE,
    /// Immediate Address
    IA,
    /// Immediate Byte (8 bit)
    IB,
    /// Immediate Byte (8 bit) + Sign Extend
    IB_SE,
    /// Immediate Word (16 bit)
    IW,
    /// Immediate Double Word (32 bit)
    IDW,
    /// Register AX
    R_AX,
    /// Register AL
    R_AL,
    /// Register AH
    R_AH,
    /// Register BX
    R_BX,
    /// Register BL
    R_BL,
    /// Register CX
    R_CX,
    /// Register CL
    R_CL,
    /// Register CH
    R_CH,
    /// Register DX
    R_DX,
    /// Register DL
    R_DL,
    /// Register DH
    R_DH,
    /// Register BP - Base Pointer
    R_BP,
    /// Register SP - Stack Pointer
    R_SP,
    /// Register SI - Source Index
    R_SI,
    /// Register DI - Destination Index
    R_DI,
    /// Register or Memory Byte
    RMB,
    /// Register or Memory Word
    RMW,
    /// Register BYTE
    RB,
    /// Register Word
    RW,
    /// Segment Register
    SR,
    /// Number of elements in this enum
    AOC_COUNT
} AddressOperandCode;

typedef struct {
    void (*op_fn)(Machine *m, Operand *rop, Operand *wop); // operation
    AddressOperandCode write_op;  // write pointer
    AddressOperandCode read_op;  //
    const char *name;        // opcode mnemonic
} Opcode;

extern const Opcode opcodes[256];
extern const Opcode opcodes_grp1[040];
extern const Opcode opcodes_grp2[040];
extern const Opcode opcodes_grp3a[010];
extern const Opcode opcodes_grp3b[010];
extern const Opcode opcodes_grp4[010];
extern const Opcode opcodes_grp5[010];


bool has_mod_rm_byte(Opcode opcode);

bool is_prefix_opcode(u8 opcode);

u8 peek_opcode(Machine *machine);

Opcode fetch_opcode(Machine *machine, u8 opcode_num);

Operand decode_read_op(Machine *machine, AddressOperandCode read_op);

Operand decode_write_op(Machine *machine, AddressOperandCode write_op);
