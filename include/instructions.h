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
        if (a & 0x8000) { // negative
            cpu->flags.OF = (sum & 0xFFFF) < 0x8000;
        } else {
            cpu->flags.OF = sum > 0x7FFF;
        }
    } else {
        cpu->flags.OF = 0;
    }
    return sum & 0xFFFF;
}

u8 add_byte(CPU *cpu, u8 src, u8 dest, u8 carry_flag) {
    u16 sum = src + dest + carry_flag;
    cpu->flags.CF = (sum & 0xFF00) > 0;
    cpu->flags.ZF = (sum & 0xFF) == 0x00;
    cpu->flags.SF = (sum & 0x80) == 0x80;
    cpu->flags.AF = (((src & 0x0f) + (dest & 0x0f)) & 0x10) == 0x10;
    cpu->flags.PF = parity(sum);

    if ((src & 0x80) == (dest & 0x80)) { // same sign
        if (src & 0x80) { // negative
            cpu->flags.OF = (sum & 0xFF) < 0x80;
        } else {
            cpu->flags.OF = sum > 0x7F;
        }
    }
    return sum & 0xFF;
}

u16 sub_word(CPU *cpu, u16 src, u16 dest, u8 borrow_flag) {
    u32 sum = dest + (~src + 1) + (~borrow_flag + 1);
    bool borrow = (sum & 0xFFFF0000) > 0;
    cpu->flags.CF = borrow;
    cpu->flags.ZF = (sum & 0xFFFF) == 0;
    cpu->flags.SF = (sum & 0x8000) == 0x8000;
    cpu->flags.AF = ((src & 0x0f) + borrow_flag) > (dest & 0x0f);
    cpu->flags.PF = parity(sum);

    if ((src & 0x8000) != (dest & 0x8000)) { // not same sign
        if ((src & 0x8000) == 0x8000) { // negative
            cpu->flags.OF = (sum & 0xFFFF) >= 0x8000;
        } else {
            cpu->flags.OF = sum <= 0x7FFF;
        }
    } else {
        cpu->flags.OF = 0;
    }
    return sum & 0xFFFF;
}

u8 sub_byte(CPU *cpu, u8 src, u8 dest, u8 borrow_flag) {
    u16 sum = dest + (~src + 1) + (~borrow_flag + 1);
    bool borrow = (sum & 0xFF00) > 0;
    cpu->flags.CF = borrow;
    cpu->flags.ZF = (sum & 0xFF) == 0;
    cpu->flags.SF = (sum & 0x80) == 0x80;
    cpu->flags.AF = ((src & 0x0f) + borrow_flag) > (dest & 0x0f);
    cpu->flags.PF = parity(sum);

    if ((src & 0x80) != (dest & 0x80)) { // not same sign
        if ((src & 0x80) == 0x80) { // negative
            cpu->flags.OF = (sum & 0xFF) >= 0x80;
        } else {
            cpu->flags.OF = sum <= 0x7F;
        }
    } else {
        cpu->flags.OF = 0;
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

void op_sub_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    *wop.word = sub_word(cpu, rop.word, *wop.word, 0);;
}

void op_sub_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    *wop.byte = sub_byte(cpu, rop.byte, *wop.byte, 0);
}

void op_sbb_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    *wop.word = sub_word(cpu, rop.word, *wop.word, cpu->flags.CF);;
}

void op_sbb_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    *wop.byte = sub_byte(cpu, rop.byte, *wop.byte, cpu->flags.CF);;
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

void op_dec_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    u16 a = *wop.word;
    u32 sum = a - 1;
    cpu->flags.ZF = (sum & 0xFFFF) == 0;
    cpu->flags.SF = (sum & 0x8000) == 0x8000;
    cpu->flags.AF = 1 > (a & 0xf);
    cpu->flags.PF = parity(sum);
    cpu->flags.OF = (a == 0x8000) ? 1 : 0;
    *wop.word = sum & 0xFFFF;
}

void op_dec_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    u8 a = *wop.byte;
    u16 sum = a - 1;
    cpu->flags.ZF = (sum & 0xFF) == 0;
    cpu->flags.SF = (sum & 0x80) == 0x80;
    cpu->flags.AF = 1 > (a & 0xf);
    cpu->flags.PF = parity(sum);
    cpu->flags.OF = (a == 0x80) ? 1 : 0;
    *wop.byte = sum & 0xFF;
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
    cpu->flags.OF = n == 1 && (carry ^ (res & 0x80)); // todo check if this is correct
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

void op_rcl_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotates of 1 and where the carry bit and MSB are different
    u16 a = *wop.word;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        u16 carry = cpu->flags.CF;
        cpu->flags.CF = (a & 0x8000) == 0x8000;
        a <<= 1;
        a |= carry;
    }

    if (n >= 1) { // todo should only be == 1 ???
        cpu->flags.OF = cpu->flags.CF ^ ((a & 0x8000) == 0x8000);
    }

    *wop.word = a;
}

void op_rcl_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotates of 1 and where the carry bit and MSB are different
    u8 a = *wop.byte;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        u16 carry = cpu->flags.CF;
        cpu->flags.CF = (a & 0x80) == 0x80;
        a <<= 1;
        a |= carry;
    }

    if (n >= 1) { // todo should only be == 1 ???
        cpu->flags.OF = cpu->flags.CF ^ ((a & 0x80) == 0x80);
    }

    *wop.byte = a;
}

void op_rcr_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotates of 1 and where the carry bit and MSB are different
    u16 a = *wop.word;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        u16 carry = cpu->flags.CF;
        cpu->flags.CF = a & 0x1;
        a >>= 1;
        a |= carry << 15;
    }

    if (n >= 1) { // todo should be n == 1 ???
        cpu->flags.OF = ((a & 0x8000) == 0x8000) ^ ((a & 0x4000) == 0x4000);
    }

    *wop.word = a;
}

void op_rcr_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotates of 1 and where the carry bit and MSB are different
    u16 a = *wop.byte;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        u16 carry = cpu->flags.CF;
        cpu->flags.CF = a & 0x1;
        a >>= 1;
        a |= carry << 7;
    }

    if (n >= 1) { // todo should be n == 1 ???
        cpu->flags.OF = ((a & 0x80) == 0x80) ^ ((a & 0x40) == 0x40);
    }

    *wop.byte = a;
}

void op_rol_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotates of 1 and where the carry bit and MSB are different
    u16 a = *wop.word;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        cpu->flags.CF = (a & 0x8000) == 0x8000;
        a <<= 1;
        a |= cpu->flags.CF;
    }

    if (n >= 1) { // todo should only be == 1 ???
        cpu->flags.OF = cpu->flags.CF ^ ((a & 0x8000) == 0x8000);
    }

    *wop.word = a;
}

void op_rol_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotates of 1 and where the carry bit and MSB are different
    u16 a = *wop.byte;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        cpu->flags.CF = (a & 0x80) == 0x80;
        a <<= 1;
        a |= cpu->flags.CF;
    }

    if (n >= 1) { // todo should only be == 1 ???
        cpu->flags.OF = cpu->flags.CF ^ ((a & 0x80) == 0x80);
    }

    *wop.byte = a;
}

void op_ror_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotates of 1 and where MSB1 and MSB2 are different
    u16 a = *wop.word;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        cpu->flags.CF = a & 0x1;
        a >>= 1;
        a |= cpu->flags.CF << 15;
    }

    if (n >= 1) { // todo should be n == 1 ???
        cpu->flags.OF = ((a & 0x8000) == 0x8000) ^ ((a & 0x4000) == 0x4000);
    }

    *wop.word = a;
}

void op_ror_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotates of 1 and where MSB1 and MSB2 are different
    u16 a = *wop.byte;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        cpu->flags.CF = a & 0x1;
        a >>= 1;
        a |= cpu->flags.CF << 7;
    }

    if (n >= 1) { // todo should be n == 1 ???
        cpu->flags.OF = ((a & 0x80) == 0x80) ^ ((a & 0x40) == 0x40);
    }

    *wop.byte = a;
}

void op_cmp_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    u16 src = rop.word;
    u16 dest = *wop.word;
    sub_word(cpu, src, dest, 0);
}

void op_cmp_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    u8 src = rop.byte;
    u8 dest = *wop.byte;
    sub_byte(cpu, src, dest, 0);
}

void op_neg_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    *wop.word = sub_word(cpu, *wop.word, 0, 0);
}

void op_neg_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    *wop.byte = sub_byte(cpu, *wop.byte, 0, 0);
}

void op_mul_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u32 res = cpu->AX * rop.word;
    cpu->DX = res >> 16;
    cpu->AX = res & 0x0000FFFF;

    cpu->flags.CF = cpu->DX != 0;
    cpu->flags.OF = cpu->DX != 0;

    // these are not supposed to be set
    cpu->flags.ZF = (res & 0xFFFF) == 0;
    cpu->flags.SF = (res & 0x8000) == 0x8000;
    cpu->flags.PF = parity(res);
}

void op_mul_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u16 res = cpu->AL * rop.byte;
    cpu->AX = res;

    cpu->flags.CF = cpu->AH != 0;
    cpu->flags.OF = cpu->AH != 0;

    // these are not supposed to be set
    cpu->flags.ZF = res == 0;
    cpu->flags.SF = (res & 0x80) == 0x80;
    cpu->flags.PF = parity(res);
}

static inline u32 sign_extend_word(u16 word) {
    return ((word & 0x8000) == 0x8000) ? 0xFFFF0000 | word : word;
}

static inline u16 sign_extend_byte(u8 byte) {
    return ((byte & 0x80) == 0x80) ? 0xFF00 | byte : byte;
}

void op_imul_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u32 src = sign_extend_word(rop.word);
    u32 dest = sign_extend_word(cpu->AX);
    u32 res = dest * src;
    cpu->DX = res >> 16;
    cpu->AX = res & 0x0000FFFF;

    u32 tmp_res = sign_extend_word(cpu->AX);
    cpu->flags.CF = res != tmp_res;
    cpu->flags.OF = res != tmp_res;

    // these are not supposed to be set
    cpu->flags.ZF = (res & 0xFFFF) == 0;
    cpu->flags.SF = (res & 0x8000) == 0x8000;
    cpu->flags.PF = parity(res);
}

void op_imul_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u16 src = sign_extend_byte(rop.byte);
    u16 dest = sign_extend_byte(cpu->AL);
    u16 res = dest * src;
    cpu->AH = res >> 8;
    cpu->AL = res & 0x00FF;

    u16 tmp_res = sign_extend_byte(cpu->AL);
    cpu->flags.CF = res != tmp_res;
    cpu->flags.OF = res != tmp_res;

    // these are not supposed to be set
    cpu->flags.ZF = (res & 0xFF) == 0;
    cpu->flags.SF = (res & 0x80) == 0x80;
    cpu->flags.PF = parity(res);
}

void op_aad(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    (void) rop;
    u8 imm8 = rop.byte;

    cpu->AL += cpu->AH * imm8; // probably undefined behavior
    cpu->AH = 0;

    cpu->flags.ZF = (cpu->AL & 0xFF) == 0;
    cpu->flags.SF = (cpu->AL & 0x80) == 0x80;
    cpu->flags.PF = parity(cpu->AL);

    // these are not supposed to be set -> undefined
    cpu->flags.CF = 0;
    cpu->flags.OF = 0;
}
