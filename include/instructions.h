#pragma once

#include <printf.h>
#include "cpu.h"

bool parity(u32 value) {
    u8 count = 0;
    for (int i = 0; i < 8; ++i) {
        count += (value >> i) & 0x1;
    }
    return !(count & 0x1);
}

void set_logical_flags_b(CPU *cpu, u8 res) {
    cpu->flags.PF = parity(res);
    cpu->flags.ZF = res == 0x00;
    cpu->flags.SF = (res & 0x80) == 0x80;
    cpu->flags.OF = 0;
}

void set_logical_flags_w(CPU *cpu, u16 res) {
    cpu->flags.PF = parity(res);
    cpu->flags.ZF = res == 0x0000;
    cpu->flags.SF = (res & 0x8000) == 0x8000;
    cpu->flags.OF = 0;
}

u16 add_word(CPU *cpu, u16 a, u16 b, u8 carry_flag) {
    u32 sum = a + b + carry_flag;
    bool carry = (sum & 0xFFFF0000) > 0;
    cpu->flags.CF = carry;
    cpu->flags.ZF = (sum & 0xFFFF) == 0;
    cpu->flags.SF = (sum & 0x8000) == 0x8000;
    cpu->flags.AF = (((a & 0x0f) + (b & 0x0f)) & 0x10) == 0x10;
    cpu->flags.PF = parity(sum);

    if ((a & 0x8000) == (b & 0x8000)) { // same sign
        if ((a & 0x8000) == 0x8000) { // negative
            cpu->flags.OF = ((a + b + carry_flag) & 0xFFFF) < 0x8000;
        } else {
            cpu->flags.OF = (a + b + carry_flag) > 0x7FFF;
        }
    } else {
        cpu->flags.OF = 0;
    }
    return sum & 0xFFFF;
}

u8 add_byte(CPU *cpu, u8 a, u8 b, u8 carry_flag) {
    u16 sum = a + b + carry_flag;
    cpu->flags.CF = (sum & 0xFF00) > 0;
    cpu->flags.ZF = (sum & 0xFF) == 0x00;
    cpu->flags.SF = (sum & 0x80) == 0x80;
    cpu->flags.AF = (((a & 0x0f) + (b & 0x0f)) & 0x10) == 0x10;
    cpu->flags.PF = parity(sum);

    if ((a & 0x80) == (b & 0x80)) { // same sign
        if ((a & 0x80)) { // negative
            cpu->flags.OF = ((a + b + carry_flag) & 0xFF) < 0x80;
        } else {
            cpu->flags.OF = (a + b + carry_flag) > 0x7F;
        }
    }
    return sum & 0xFF;
}

void op_mov_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) cpu;
    *wop.word = rop.word;
}

void op_mov_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) cpu;
    *wop.byte = rop.byte;
}

void op_add_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    *wop.byte = add_byte(cpu, rop.byte, *wop.byte, 0);
}

void op_add_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    *wop.word = add_word(cpu, rop.word, *wop.word, 0);
}

void op_adc_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    *wop.byte = add_byte(cpu, rop.byte, *wop.byte, cpu->flags.CF);
}

void op_adc_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    *wop.word = add_word(cpu, rop.word, *wop.word, cpu->flags.CF);
}

void op_and_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    u16 src = rop.word;
    u16 dest = *wop.word;
    u16 res = dest & src;

    set_logical_flags_w(cpu, res);
    *wop.word = res;
}

void op_and_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    u8 src = rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest & src;

//    printf("AND 0x%02X & 0x%02X = 0x%02X\n", src, dest, res);

    set_logical_flags_b(cpu, res);
    *wop.byte = res;
}

void op_or_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    u16 src = rop.word;
    u16 dest = *wop.word;
    u16 res = dest | src;

    set_logical_flags_w(cpu, res);
    *wop.word = res;
}

void op_or_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    u8 src = rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest | src;

    set_logical_flags_b(cpu, res);
    *wop.byte = res;
}

void op_xor_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    u16 src = rop.word;
    u16 dest = *wop.word;
    u16 res = dest ^ src;

    set_logical_flags_w(cpu, res);
    *wop.word = res;
}


void op_xor_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    u8 src = rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest ^ src;

    set_logical_flags_b(cpu, res);
    *wop.byte = res;
}

void op_test_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    u16 src = rop.word;
    u16 dest = *wop.word;
    u16 res = dest & src;

    set_logical_flags_w(cpu, res);
}

void op_not_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) cpu;
    (void) rop;
    *wop.word = ~(*wop.word);
}

void op_not_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) cpu;
    (void) rop;
    *wop.byte = ~(*wop.byte);
}

void op_test_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    u8 src = rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest & src;

    set_logical_flags_b(cpu, res);
}

void op_jmp(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    cpu->IP += (s8) rop.byte;
}

void op_pushf(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    cpu->SP -= 2;
    *(wop.word - 1) = cpu->flags.word;
}

void op_popf(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    cpu->SP += 2;
    cpu->flags.word = (rop.word & 0x0FD5) | 0x2; // clear unused bits and set 2 bit to 1
}

void op_push(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    cpu->SP -= 2;
    *(wop.word - 1) = rop.word;
}

void op_pop(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    cpu->SP += 2;
    *wop.word = rop.word;
}

void op_hlt(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    cpu->halted = true;
}

void op_clc(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    cpu->flags.CF = 0;
}

void op_cld(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    cpu->flags.DF = 0;
}

void op_cli(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    cpu->flags.IF = 0;
}

void op_cmc(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    cpu->flags.CF = !cpu->flags.CF;
}

void op_stc(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    cpu->flags.CF = 1;
}

void op_std(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    cpu->flags.DF = 1;
}

void op_sti(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    cpu->flags.IF = 1;
}

void op_nop(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) cpu;
    (void) rop;
    (void) wop;
}

void op_inc_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    uint8_t a = *wop.byte;
    u16 sum = a + 1;
    cpu->flags.ZF = (sum & 0xFF) == 0;
    cpu->flags.SF = (sum & 0x80) == 0x80;
    cpu->flags.AF = (((a & 0x0f) + 1) & 0x10) == 0x10;
    cpu->flags.PF = parity(sum);
    cpu->flags.OF = (a == 0x7F) ? 1 : 0;

    *wop.byte = sum & 0xFF;
}

void op_inc_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    u16 a = *wop.word;
    u32 sum = a + 1;
    cpu->flags.ZF = (sum & 0xFFFF) == 0;
    cpu->flags.SF = (sum & 0x8000) == 0x8000;
    cpu->flags.AF = (((a & 0x0f) + 1) & 0x10) == 0x10;
    cpu->flags.PF = parity(sum);
    cpu->flags.OF = (a == 0x7FFF) ? 1 : 0;
    *wop.word = sum & 0xFFFF;
}

void op_shl_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // overflow applies to shifts of 1 and where the carry bit and MSB are different
    u16 a = *wop.word;
    u8 n = rop.word;
    if (n == 0) {
        return;
    }
    u16 res = a << n;
    bool carry = (a >> (16 - n)) & 0x1;
    cpu->flags.CF = carry;
    cpu->flags.ZF = (res & 0xFFFF) == 0;
    cpu->flags.SF = (res & 0x8000) == 0x8000;
    cpu->flags.OF = n == 1 && (carry ^ (res & 0x8000));
    cpu->flags.PF = parity(res);
    *wop.word = res;
}

void op_shl_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // overflow applies to shifts of 1 and where the carry bit and MSB are different
    u8 a = *wop.byte;
    u8 n = rop.byte;
    if (n == 0) {
        return;
    }
    u8 res = a << n;
    bool carry = (a >> (8 - n)) & 0x1;
    cpu->flags.CF = carry;
    cpu->flags.ZF = (res & 0xFF) == 0;
    cpu->flags.SF = (res & 0x80) == 0x80;
    cpu->flags.OF = n == 1 && (carry ^ (res & 0x80));
    cpu->flags.PF = parity(res);
    *wop.byte = res;
}

void op_sar_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // Need to do sign extension on SAR
    // Overflow is cleared on shift by 1
    u16 a = *wop.word;
    u8 n = rop.word;
    if (n == 0) {
        return;
    }
    u16 sign = a & 0x8000;
    u16 res = a;
    bool carry = false;
    for (int i = 0; i < n; ++i) {
        carry = res & 0x1;
        res >>= 1;
        res |= sign;
    }

    cpu->flags.CF = carry;
    cpu->flags.ZF = (res & 0xFFFF) == 0;
    cpu->flags.SF = (res & 0x8000) == 0x8000;
    if (n == 1) {
        cpu->flags.OF = 0;
    }
    cpu->flags.PF = parity(res);
    *wop.word = res;
}

void op_sar_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // Need to do sign extension on SAR
    // Overflow is cleared on shift by 1
    u8 a = *wop.byte;
    u8 n = rop.byte;
    if (n == 0) {
        return;
    }
    u8 sign = a & 0x80;
    u8 res = a;
    bool carry = false;
    for (int i = 0; i < n; ++i) {
        carry = res & 0x1;
        res >>= 1;
        res |= sign;
    }

    cpu->flags.CF = carry;
    cpu->flags.ZF = (res & 0xFF) == 0;
    cpu->flags.SF = (res & 0x80) == 0x80;
    if (n == 1) {
        cpu->flags.OF = 0;
    }
    cpu->flags.PF = parity(res);
    *wop.byte = res;
}

void op_shr_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // Overflow is set to MSB of operand
    u16 a = *wop.word;
    u8 n = rop.word;
    if (n == 0) {
        return;
    }
    u16 res = a >> n;

    cpu->flags.CF = (a >> (n - 1)) & 0x1;
    cpu->flags.ZF = (res & 0xFFFF) == 0;
    cpu->flags.SF = (res & 0x8000) == 0x8000;
    cpu->flags.OF = (a & 0x8000) == 0x8000;
    cpu->flags.PF = parity(res);
    *wop.word = res;
}

void op_shr_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // Overflow is set to MSB of operand
    u8 a = *wop.byte;
    u8 n = rop.byte;
    if (n == 0) {
        return;
    }
    u8 res = a >> n;

    cpu->flags.CF = (a >> (n - 1)) & 0x1;
    cpu->flags.ZF = (res & 0xFF) == 0;
    cpu->flags.SF = (res & 0x80) == 0x80;
    cpu->flags.OF = (a & 0x80) == 0x80;
    cpu->flags.PF = parity(res);
    *wop.byte = res;
}
