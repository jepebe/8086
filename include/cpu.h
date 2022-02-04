#pragma once

#include <stdbool.h>
#include "80x86.h"
#include "memory.h"

typedef struct {
    union {
        u8 opcode;
        struct {
            u8 reg_mem: 3;
            u8 reg_sreg: 3;
            u8 mode: 2;
        };
    };
} AddrMode;

typedef enum {
    BYTE,
    WORD
} OperandSize;

typedef union {
    u16 word; // status register
    struct {
        u8 CF: 1; // Carry Flag
        u8  : 1; // Not Used
        u8 PF: 1; // Parity Flag
        u8  : 1; // Not Used
        u8 AF: 1; // Auxiliary Carry Flag
        u8  : 1; // Not Used
        u8 ZF: 1; // Zero Flag
        u8 SF: 1; // Sign Flag
        u8 TF: 1; // Trap Flag
        u8 IF: 1; // Interrupt Flag
        u8 DF: 1; // Direction Flag
        u8 OF: 1; // Overflow Flag
        u8  : 4; // Not used
    };
} Flags;

typedef struct {
    // union __attribute__((__packed__)) {

    // Main registers
    union {
        u16 AX; // primary accumulator
        struct {
            u8 AL;  // ax low byte
            u8 AH;  // ax high byte
        };
    };

    union {
        u16 BX; // base, accumulator
        struct {
            u8 BL; // bx low byte
            u8 BH; // bx high byte
        };
    };

    union {
        u16 CX; // counter, accumulator
        struct {
            u8 CL; // cx low byte
            u8 CH; // cx high byte
        };
    };

    union {
        u16 DX; // accumulator, extended accumulator
        struct {
            u8 DL;  // dx low byte
            u8 DH;  // dx high byte
        };
    };

    // Index registers

    u16 SI; // source index
    u16 DI; // destination index
    u16 BP; // base pointer
    u16 SP; // stack pointer

    // program counter

    u16 IP; // instruction pointer

    // segment registers

    u16 CS; // code segment
    u16 DS; // data segment
    u16 ES; // extra segment
    u16 SS; // stack segment


    // status register
    Flags flags;

    u8 opcode;
    AddrMode addr_mode;
    bool halted;
    u16 immediate_write;
    u16 immediate_read;
} CPU;

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

typedef struct {
    union {
        u16 word;
        u8 byte;
    };
} ReadOperand;

typedef struct {
    union {
        u16 *word;
        u8 *byte;
    };
} WriteOperand;

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
    /// Pointer to what SP is pointing to
    SP_PTR,
    /// Number of elements in this enum
    AOC_COUNT
} AddressOperandCode;

typedef struct {
    void (*op_fn)(CPU *cpu, ReadOperand rop, WriteOperand wop); // operation
    AddressOperandCode write_op;
    AddressOperandCode read_op;
    const char *name;        // opcode mnemonic

} Opcode;

void cpu_tick(CPU *cpu, Memory *memory);

void cpu_reset(CPU *cpu);

/// Correctly return the IP with the CS offset
u32 cpu_ip(CPU *cpu);

/// Correctly return an address with the DS offset
u32 cpu_ds(CPU *cpu, u16 offset);

/// Returns the SP location after SS adjustment
u32 cpu_sp(CPU *cpu);

char *flags_to_str(Flags flags);

NO_RETURN void cpu_error(CPU *cpu, char *message, int value);

void cpu_note_int(CPU *cpu, char *message, int value);

void cpu_note_str(CPU *cpu, char *message, char *value);

void cpu_error_marker(CPU *cpu, char *file, int line);

void cpu_peek(CPU *cpu, Memory *mem);

NO_RETURN void cpu_error_int(CPU *cpu, Memory *memory, char *message, int value);

NO_RETURN void cpu_error_str(CPU *cpu, Memory *memory, char *message, char *value);

void cpu_instruction_context(CPU *cpu, Memory *memory);

u16 read_memory_u16(u32 addr, Memory *memory);

u8 read_memory_u8(u32 addr, Memory *memory);

void dump_cpu(CPU *cpu, Memory *memory);

void dump_ram(CPU *cpu, Memory *memory);

