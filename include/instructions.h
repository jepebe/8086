#pragma once

#include <printf.h>
#include "debug.h"
#include "machine.h"

bool parity(u32 value) {
    u8 count = 0;
    for (int i = 0; i < 8; ++i) {
        count += (value >> i) & 0x1;
    }
    return !(count & 0x1);
}

void set_logical_flags_b(Machine *m, u8 res) {
    m->cpu->flags.PF = parity(res);
    m->cpu->flags.ZF = res == 0x00;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.OF = 0;
}

void set_logical_flags_w(Machine *m, u16 res) {
    m->cpu->flags.PF = parity(res);
    m->cpu->flags.ZF = res == 0x0000;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.OF = 0;
}

u16 add_word(Machine *m, u16 a, u16 b, u8 carry_flag) {
    u32 sum = a + b + carry_flag;
    bool carry = (sum & 0xFFFF0000) > 0;
    m->cpu->flags.CF = carry;
    m->cpu->flags.ZF = (sum & 0xFFFF) == 0;
    m->cpu->flags.SF = (sum & 0x8000) == 0x8000;
    m->cpu->flags.AF = (((a & 0x0f) + (b & 0x0f)) & 0x10) == 0x10;
    m->cpu->flags.PF = parity(sum);

    if ((a & 0x8000) == (b & 0x8000)) { // same sign
        if (a & 0x8000) { // negative
            m->cpu->flags.OF = (sum & 0xFFFF) < 0x8000;
        } else {
            m->cpu->flags.OF = sum > 0x7FFF;
        }
    } else {
        m->cpu->flags.OF = 0;
    }
    return sum & 0xFFFF;
}

u8 add_byte(Machine *m, u8 src, u8 dest, u8 carry_flag) {
    u16 sum = src + dest + carry_flag;
    m->cpu->flags.CF = (sum & 0xFF00) > 0;
    m->cpu->flags.ZF = (sum & 0xFF) == 0x00;
    m->cpu->flags.SF = (sum & 0x80) == 0x80;
    m->cpu->flags.AF = (((src & 0x0f) + (dest & 0x0f)) & 0x10) == 0x10;
    m->cpu->flags.PF = parity(sum);

    if ((src & 0x80) == (dest & 0x80)) { // same sign
        if (src & 0x80) { // negative
            m->cpu->flags.OF = (sum & 0xFF) < 0x80;
        } else {
            m->cpu->flags.OF = sum > 0x7F;
        }
    }
    return sum & 0xFF;
}

u16 sub_word(Machine *m, u16 src, u16 dest, u8 borrow_flag) {
    u32 sum = dest + (~src + 1) + (~borrow_flag + 1);
    bool borrow = (sum & 0xFFFF0000) > 0;
    m->cpu->flags.CF = borrow;
    m->cpu->flags.ZF = (sum & 0xFFFF) == 0;
    m->cpu->flags.SF = (sum & 0x8000) == 0x8000;
    m->cpu->flags.AF = ((src & 0x0f) + borrow_flag) > (dest & 0x0f);
    m->cpu->flags.PF = parity(sum);

    if ((src & 0x8000) != (dest & 0x8000)) { // not same sign
        if ((src & 0x8000) == 0x8000) { // negative
            m->cpu->flags.OF = (sum & 0xFFFF) >= 0x8000;
        } else {
            m->cpu->flags.OF = sum <= 0x7FFF;
        }
    } else {
        m->cpu->flags.OF = 0;
    }
    return sum & 0xFFFF;
}

u8 sub_byte(Machine *m, u8 src, u8 dest, u8 borrow_flag) {
    u16 sum = dest + (~src + 1) + (~borrow_flag + 1);
    bool borrow = (sum & 0xFF00) > 0;
    m->cpu->flags.CF = borrow;
    m->cpu->flags.ZF = (sum & 0xFF) == 0;
    m->cpu->flags.SF = (sum & 0x80) == 0x80;
    m->cpu->flags.AF = ((src & 0x0f) + borrow_flag) > (dest & 0x0f);
    m->cpu->flags.PF = parity(sum);

    if ((src & 0x80) != (dest & 0x80)) { // not same sign
        if ((src & 0x80) == 0x80) { // negative
            m->cpu->flags.OF = (sum & 0xFF) >= 0x80;
        } else {
            m->cpu->flags.OF = sum <= 0x7F;
        }
    } else {
        m->cpu->flags.OF = 0;
    }
    return sum & 0xFF;
}

void op_mov_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) m;
    *wop.word = rop.word;
}

void op_mov_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) m;
    *wop.byte = rop.byte;
}

void op_add_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    *wop.byte = add_byte(m, rop.byte, *wop.byte, 0);
}

void op_add_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    *wop.word = add_word(m, rop.word, *wop.word, 0);
}

void op_adc_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    *wop.byte = add_byte(m, rop.byte, *wop.byte, m->cpu->flags.CF);
}

void op_adc_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    *wop.word = add_word(m, rop.word, *wop.word, m->cpu->flags.CF);
}

void op_sub_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    *wop.word = sub_word(m, rop.word, *wop.word, 0);
}

void op_sub_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    *wop.byte = sub_byte(m, rop.byte, *wop.byte, 0);
}

void op_sbb_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    *wop.word = sub_word(m, rop.word, *wop.word, m->cpu->flags.CF);
}

void op_sbb_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    *wop.byte = sub_byte(m, rop.byte, *wop.byte, m->cpu->flags.CF);
}

void op_and_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    u16 src = rop.word;
    u16 dest = *wop.word;
    u16 res = dest & src;

    set_logical_flags_w(m, res);
    *wop.word = res;
}

void op_and_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    u8 src = rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest & src;

    set_logical_flags_b(m, res);
    *wop.byte = res;
}

void op_or_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    u16 src = rop.word;
    u16 dest = *wop.word;
    u16 res = dest | src;

    set_logical_flags_w(m, res);
    *wop.word = res;
}

void op_or_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    u8 src = rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest | src;

    set_logical_flags_b(m, res);
    *wop.byte = res;
}

void op_xor_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    u16 src = rop.word;
    u16 dest = *wop.word;
    u16 res = dest ^ src;

    set_logical_flags_w(m, res);
    *wop.word = res;
}


void op_xor_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    u8 src = rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest ^ src;

    set_logical_flags_b(m, res);
    *wop.byte = res;
}

void op_test_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    u16 src = rop.word;
    u16 dest = *wop.word;
    u16 res = dest & src;

    set_logical_flags_w(m, res);
}

void op_not_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) m;
    (void) rop;
    *wop.word = ~(*wop.word);
}

void op_not_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) m;
    (void) rop;
    *wop.byte = ~(*wop.byte);
}

void op_test_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    u8 src = rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest & src;

    set_logical_flags_b(m, res);
}

void op_jmp_short(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    m->cpu->IP += (s8) rop.byte;
}

void op_jmp_near(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    m->cpu->IP = rop.word;
}

void op_jz(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    if (m->cpu->flags.ZF == 1) {
        m->cpu->IP += (s8) rop.byte;
    }
}

/// Returns a pointer to the top of the stack
u16 *stack_pointer(Machine *machine) {
    u32 sp = cpu_sp(machine->cpu);
    return (u16 *) &machine->memory->ram[sp];
}

/// Returns the value at the top of the stack
u16 peek_stack(Machine *machine) {
    return read_memory_u16(cpu_sp(machine->cpu), machine->memory);
}

void op_pushf(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->SP -= 2;
    u16 *ptr = stack_pointer(m);
    *(ptr) = m->cpu->flags.word;
}

void op_popf(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    (void) rop;
    m->cpu->flags.word = (peek_stack(m) & 0x0FD5) | 0x2; // clear unused bits and set 2 bit to 1
    m->cpu->SP += 2;
}

void op_push(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    m->cpu->SP -= 2;
    u16 *ptr = stack_pointer(m);
    *(ptr) = rop.word;
}

void op_pop(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    *wop.word = peek_stack(m);
    m->cpu->SP += 2;
}

void op_hlt(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->halted = true;
}

void op_clc(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.CF = 0;
}

void op_cld(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.DF = 0;
}

void op_cli(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.IF = 0;
}

void op_cmc(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.CF = !m->cpu->flags.CF;
}

void op_stc(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.CF = 1;
}

void op_std(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.DF = 1;
}

void op_sti(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.IF = 1;
}

void op_nop(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) m;
    (void) rop;
    (void) wop;
}

void op_inc_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    uint8_t a = *wop.byte;
    u16 sum = a + 1;
    m->cpu->flags.ZF = (sum & 0xFF) == 0;
    m->cpu->flags.SF = (sum & 0x80) == 0x80;
    m->cpu->flags.AF = (((a & 0x0f) + 1) & 0x10) == 0x10;
    m->cpu->flags.PF = parity(sum);
    m->cpu->flags.OF = (a == 0x7F) ? 1 : 0;

    *wop.byte = sum & 0xFF;
}

void op_inc_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    u16 a = *wop.word;
    u32 sum = a + 1;
    m->cpu->flags.ZF = (sum & 0xFFFF) == 0;
    m->cpu->flags.SF = (sum & 0x8000) == 0x8000;
    m->cpu->flags.AF = (((a & 0x0f) + 1) & 0x10) == 0x10;
    m->cpu->flags.PF = parity(sum);
    m->cpu->flags.OF = (a == 0x7FFF) ? 1 : 0;
    *wop.word = sum & 0xFFFF;
}

void op_dec_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    u16 a = *wop.word;
    u32 sum = a - 1;
    m->cpu->flags.ZF = (sum & 0xFFFF) == 0;
    m->cpu->flags.SF = (sum & 0x8000) == 0x8000;
    m->cpu->flags.AF = 1 > (a & 0xf);
    m->cpu->flags.PF = parity(sum);
    m->cpu->flags.OF = (a == 0x8000) ? 1 : 0;
    *wop.word = sum & 0xFFFF;
}

void op_dec_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    u8 a = *wop.byte;
    u16 sum = a - 1;
    m->cpu->flags.ZF = (sum & 0xFF) == 0;
    m->cpu->flags.SF = (sum & 0x80) == 0x80;
    m->cpu->flags.AF = 1 > (a & 0xf);
    m->cpu->flags.PF = parity(sum);
    m->cpu->flags.OF = (a == 0x80) ? 1 : 0;
    *wop.byte = sum & 0xFF;
}

void op_shl_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    // overflow applies to shift of 1 and where the carry bit and MSB are different
    u16 a = *wop.word;
    u8 n = rop.word;
    if (n == 0) {
        return;
    }
    u16 res = a << n;
    bool carry = (a >> (16 - n)) & 0x1;
    m->cpu->flags.CF = carry;
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.OF = n == 1 && (carry ^ (res & 0x8000));
    m->cpu->flags.PF = parity(res);
    *wop.word = res;
}

void op_shl_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    // overflow applies to shift of 1 and where the carry bit and MSB are different
    u8 a = *wop.byte;
    u8 n = rop.byte;
    if (n == 0) {
        return;
    }
    u8 res = a << n;
    bool carry = (a >> (8 - n)) & 0x1;
    m->cpu->flags.CF = carry;
    m->cpu->flags.ZF = (res & 0xFF) == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.OF = n == 1 && (carry ^ (res & 0x80)); // todo check if this is correct
    m->cpu->flags.PF = parity(res);
    *wop.byte = res;
}

void op_sar_w(Machine *m, ReadOperand rop, WriteOperand wop) {
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

    m->cpu->flags.CF = carry;
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    if (n == 1) {
        m->cpu->flags.OF = 0;
    }
    m->cpu->flags.PF = parity(res);
    *wop.word = res;
}

void op_sar_b(Machine *m, ReadOperand rop, WriteOperand wop) {
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

    m->cpu->flags.CF = carry;
    m->cpu->flags.ZF = (res & 0xFF) == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    if (n == 1) {
        m->cpu->flags.OF = 0;
    }
    m->cpu->flags.PF = parity(res);
    *wop.byte = res;
}

void op_shr_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    // Overflow is set to MSB of operand
    u16 a = *wop.word;
    u8 n = rop.word;
    if (n == 0) {
        return;
    }
    u16 res = a >> n;

    m->cpu->flags.CF = (a >> (n - 1)) & 0x1;
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.OF = (a & 0x8000) == 0x8000;
    m->cpu->flags.PF = parity(res);
    *wop.word = res;
}

void op_shr_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    // Overflow is set to MSB of operand
    u8 a = *wop.byte;
    u8 n = rop.byte;
    if (n == 0) {
        return;
    }
    u8 res = a >> n;

    m->cpu->flags.CF = (a >> (n - 1)) & 0x1;
    m->cpu->flags.ZF = (res & 0xFF) == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.OF = (a & 0x80) == 0x80;
    m->cpu->flags.PF = parity(res);
    *wop.byte = res;
}

void op_rcl_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotate of 1 and where the carry bit and MSB are different
    u16 a = *wop.word;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        u16 carry = m->cpu->flags.CF;
        m->cpu->flags.CF = (a & 0x8000) == 0x8000;
        a <<= 1;
        a |= carry;
    }

    if (n >= 1) { // todo should only be == 1 ???
        m->cpu->flags.OF = m->cpu->flags.CF ^ ((a & 0x8000) == 0x8000);
    }

    *wop.word = a;
}

void op_rcl_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotate of 1 and where the carry bit and MSB are different
    u8 a = *wop.byte;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        u16 carry = m->cpu->flags.CF;
        m->cpu->flags.CF = (a & 0x80) == 0x80;
        a <<= 1;
        a |= carry;
    }

    if (n >= 1) { // todo should only be == 1 ???
        m->cpu->flags.OF = m->cpu->flags.CF ^ ((a & 0x80) == 0x80);
    }

    *wop.byte = a;
}

void op_rcr_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotate of 1 and where the carry bit and MSB are different
    u16 a = *wop.word;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        u16 carry = m->cpu->flags.CF;
        m->cpu->flags.CF = a & 0x1;
        a >>= 1;
        a |= carry << 15;
    }

    if (n >= 1) { // todo should be n == 1 ???
        m->cpu->flags.OF = ((a & 0x8000) == 0x8000) ^ ((a & 0x4000) == 0x4000);
    }

    *wop.word = a;
}

void op_rcr_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotate of 1 and where the carry bit and MSB are different
    u16 a = *wop.byte;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        u16 carry = m->cpu->flags.CF;
        m->cpu->flags.CF = a & 0x1;
        a >>= 1;
        a |= carry << 7;
    }

    if (n >= 1) { // todo should be n == 1 ???
        m->cpu->flags.OF = ((a & 0x80) == 0x80) ^ ((a & 0x40) == 0x40);
    }

    *wop.byte = a;
}

void op_rol_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotate of 1 and where the carry bit and MSB are different
    u16 a = *wop.word;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        m->cpu->flags.CF = (a & 0x8000) == 0x8000;
        a <<= 1;
        a |= m->cpu->flags.CF;
    }

    if (n >= 1) { // todo should only be == 1 ???
        m->cpu->flags.OF = m->cpu->flags.CF ^ ((a & 0x8000) == 0x8000);
    }

    *wop.word = a;
}

void op_rol_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotate of 1 and where the carry bit and MSB are different
    u16 a = *wop.byte;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        m->cpu->flags.CF = (a & 0x80) == 0x80;
        a <<= 1;
        a |= m->cpu->flags.CF;
    }

    if (n >= 1) { // todo should only be == 1 ???
        m->cpu->flags.OF = m->cpu->flags.CF ^ ((a & 0x80) == 0x80);
    }

    *wop.byte = a;
}

void op_ror_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotate of 1 and where MSB1 and MSB2 are different
    u16 a = *wop.word;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        m->cpu->flags.CF = a & 0x1;
        a >>= 1;
        a |= m->cpu->flags.CF << 15;
    }

    if (n >= 1) { // todo should be n == 1 ???
        m->cpu->flags.OF = ((a & 0x8000) == 0x8000) ^ ((a & 0x4000) == 0x4000);
    }

    *wop.word = a;
}

void op_ror_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotate of 1 and where MSB1 and MSB2 are different
    u16 a = *wop.byte;
    u8 n = rop.byte & 0x1F;
    if (n == 0) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        m->cpu->flags.CF = a & 0x1;
        a >>= 1;
        a |= m->cpu->flags.CF << 7;
    }

    if (n >= 1) { // todo should be n == 1 ???
        m->cpu->flags.OF = ((a & 0x80) == 0x80) ^ ((a & 0x40) == 0x40);
    }

    *wop.byte = a;
}

void op_cmp_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    u16 src = rop.word;
    u16 dest = *wop.word;
    sub_word(m, src, dest, 0);
}

void op_cmp_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    u8 src = rop.byte;
    u8 dest = *wop.byte;
    sub_byte(m, src, dest, 0);
}

void op_neg_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    *wop.word = sub_word(m, *wop.word, 0, 0);
}

void op_neg_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    *wop.byte = sub_byte(m, *wop.byte, 0, 0);
}

void op_mul_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u32 res = m->cpu->AX * rop.word;
    m->cpu->DX = res >> 16;
    m->cpu->AX = res & 0x0000FFFF;

    m->cpu->flags.CF = m->cpu->DX != 0;
    m->cpu->flags.OF = m->cpu->DX != 0;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.PF = parity(res);
}

void op_mul_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u16 res = m->cpu->AL * rop.byte;
    m->cpu->AX = res;

    m->cpu->flags.CF = m->cpu->AH != 0;
    m->cpu->flags.OF = m->cpu->AH != 0;

    // these are not supposed to be set
    m->cpu->flags.ZF = res == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.PF = parity(res);
}

static inline u32 sign_extend_word(u16 word) {
    return ((word & 0x8000) == 0x8000) ? 0xFFFF0000 | word : word;
}

static inline u16 sign_extend_byte(u8 byte) {
    return ((byte & 0x80) == 0x80) ? 0xFF00 | byte : byte;
}

void op_imul_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u32 src = sign_extend_word(rop.word);
    u32 dest = sign_extend_word(m->cpu->AX);
    u32 res = dest * src;
    m->cpu->DX = res >> 16;
    m->cpu->AX = res & 0x0000FFFF;

    u32 tmp_res = sign_extend_word(m->cpu->AX);
    m->cpu->flags.CF = res != tmp_res;
    m->cpu->flags.OF = res != tmp_res;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.PF = parity(res);
}

void op_imul_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u16 src = sign_extend_byte(rop.byte);
    u16 dest = sign_extend_byte(m->cpu->AL);
    u16 res = dest * src;
    m->cpu->AH = res >> 8;
    m->cpu->AL = res & 0x00FF;

    u16 tmp_res = sign_extend_byte(m->cpu->AL);
    m->cpu->flags.CF = res != tmp_res;
    m->cpu->flags.OF = res != tmp_res;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFF) == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.PF = parity(res);
}

void op_aad(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    (void) rop;
    u8 imm8 = rop.byte;

    m->cpu->AL += m->cpu->AH * imm8; // probably undefined behavior
    m->cpu->AH = 0;

    m->cpu->flags.ZF = (m->cpu->AL & 0xFF) == 0;
    m->cpu->flags.SF = (m->cpu->AL & 0x80) == 0x80;
    m->cpu->flags.PF = parity(m->cpu->AL);

    // these are not supposed to be set -> undefined
    m->cpu->flags.CF = 0;
    m->cpu->flags.OF = 0;
}

void op_int(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u8 interrupt = rop.byte;

    op_pushf(m, (ReadOperand) {0}, (WriteOperand) {0});
    m->cpu->flags.IF = 0;
    m->cpu->flags.TF = 0;
    op_push(m, (ReadOperand) {.word=m->cpu->CS}, (WriteOperand) {0});
    op_push(m, (ReadOperand) {.word=m->cpu->IP}, (WriteOperand) {0});

    m->cpu->IP = read_memory_u16(interrupt * 4, m->memory);
    m->cpu->CS = read_memory_u16(interrupt * 4 + 2, m->memory);
}

void op_iret(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    op_pop(m, (ReadOperand) {0}, (WriteOperand) {.word=&m->cpu->IP});
    op_pop(m, (ReadOperand) {0}, (WriteOperand) {.word=&m->cpu->CS});
    op_popf(m, (ReadOperand) {0}, (WriteOperand) {0});
}

void op_into(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;

    if (m->cpu->flags.OF) {
        op_int(m, (ReadOperand) {.byte=4}, (WriteOperand) {0});
    }
}

void division_error(Machine *m) {
    (void) m;
    m->cpu->IP = m->cpu->opcode_ip; // @286 compatibility with tests
    op_int(m, (ReadOperand) {.byte=0}, (WriteOperand) {0});

    cpu_note_int(m, "Division Error", 0);
}

void op_aam(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u8 imm8 = rop.byte;

    if (imm8 == 0) {
        division_error(m);
        return;
    }

    u8 temp_al = m->cpu->AL;
    m->cpu->AH = temp_al / imm8; // probably undefined behavior
    m->cpu->AL = temp_al % imm8;

    m->cpu->flags.ZF = (m->cpu->AL & 0xFF) == 0;
    m->cpu->flags.SF = (m->cpu->AL & 0x80) == 0x80;
    m->cpu->flags.PF = parity(m->cpu->AL);

    // these are not supposed to be set -> undefined
    m->cpu->flags.CF = 0;
    m->cpu->flags.OF = 0;
}

void op_div_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u32 src = rop.word;

    if (src == 0x0) {
        division_error(m);
        return;
    }

    u32 dest = (m->cpu->DX << 16) | m->cpu->AX;
    u32 res = dest / src;

    if (res > 0xFFFF) {
        division_error(m);
        return;
    }

    m->cpu->AX = res & 0x0000FFFF;
    m->cpu->DX = dest % src;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.PF = parity(res);
}

void op_div_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    u16 src = rop.byte;

    if (src == 0x0) {
        division_error(m);
        return;
    }

    u16 dest = m->cpu->AX;
    u16 res = dest / src;

    if (res > 0xFF) {
        division_error(m);
        return;
    }

    m->cpu->AL = res & 0x00FF;
    m->cpu->AH = dest % src;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFF) == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.PF = parity(res);
}

void op_idiv_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    s32 src = (s32) sign_extend_word(rop.word);

    if (src == 0x0) {
        division_error(m);
        return;
    }

    s32 dest = (m->cpu->DX << 16) | m->cpu->AX;
    s32 res = dest / src;

    if (res < -0x7FFF || res > 0x7FFF) {
        division_error(m);
        return;
    }

    m->cpu->AX = res & 0x0000FFFF;
    m->cpu->DX = dest % src;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.PF = parity(res);
}

void op_idiv_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    s16 src = (s16) sign_extend_byte(rop.byte);

    if (src == 0x0) {
        division_error(m);
        return;
    }

    s16 dest = (s16) m->cpu->AX;
    // https://clang.llvm.org/extra/clang-tidy/checks/cppcoreguidelines-narrowing-conversions.html
    s16 res = dest / src;  // todo??? NOLINT(cppcoreguidelines-narrowing-conversions)

    if (res < -0x7F || res > 0x7F) {
        division_error(m);
        return;
    }

    m->cpu->AL = res & 0x00FF;
    m->cpu->AH = dest % src;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFF) == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.PF = parity(res);
}

void op_ret(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    op_pop(m, (ReadOperand) {0}, (WriteOperand) {.word=&m->cpu->IP});
}

void op_cmps_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    u16 src1 = read_memory_u16(cpu_ds(m->cpu, m->cpu->SI), m->memory);
    u16 src2 = read_memory_u16(cpu_es(m->cpu, m->cpu->DI), m->memory);
    sub_word(m, src2, src1, 0);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 2;
        m->cpu->DI += 2;
    } else {
        m->cpu->SI -= 2;
        m->cpu->DI -= 2;
    }
}

void op_cmps_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    u8 src1 = read_memory_u8(cpu_ds(m->cpu, m->cpu->SI), m->memory);
    u8 src2 = read_memory_u8(cpu_es(m->cpu, m->cpu->DI), m->memory);
    sub_byte(m, src2, src1, 0);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 1;
        m->cpu->DI += 1;
    } else {
        m->cpu->SI -= 1;
        m->cpu->DI -= 1;
    }
}

void op_lods_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->AX = read_memory_u16(cpu_ds(m->cpu, m->cpu->SI), m->memory);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 2;
        m->cpu->DI += 2;
    } else {
        m->cpu->SI -= 2;
        m->cpu->DI -= 2;
    }
}

void op_lods_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->AL = read_memory_u8(cpu_ds(m->cpu, m->cpu->SI), m->memory);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 1;
        m->cpu->DI += 1;
    } else {
        m->cpu->SI -= 1;
        m->cpu->DI -= 1;
    }
}

void op_movs_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    u32 from_addr = cpu_ds(m->cpu, m->cpu->SI);
    u16 word = read_memory_u16(from_addr, m->memory);
    u32 to_addr = cpu_es(m->cpu, m->cpu->DI);
    write_memory_u16(to_addr, m->memory, word);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 2;
        m->cpu->DI += 2;
    } else {
        m->cpu->SI -= 2;
        m->cpu->DI -= 2;
    }
}

void op_movs_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    u32 from_addr = cpu_ds(m->cpu, m->cpu->SI);
    u8 byte = read_memory_u8(from_addr, m->memory);
    u32 to_addr = cpu_es(m->cpu, m->cpu->DI);
    write_memory_u8(to_addr, m->memory, byte);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 1;
        m->cpu->DI += 1;
    } else {
        m->cpu->SI -= 1;
        m->cpu->DI -= 1;
    }
}

void op_scas_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    u16 src = read_memory_u16(cpu_es(m->cpu, m->cpu->DI), m->memory);
    u16 dest = m->cpu->AX;
    sub_word(m, src, dest, 0);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 2;
        m->cpu->DI += 2;
    } else {
        m->cpu->SI -= 2;
        m->cpu->DI -= 2;
    }
}

void op_scas_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    u8 src = read_memory_u8(cpu_es(m->cpu, m->cpu->DI), m->memory);
    u8 dest = m->cpu->AL;
    sub_byte(m, src, dest, 0);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 1;
        m->cpu->DI += 1;
    } else {
        m->cpu->SI -= 1;
        m->cpu->DI -= 1;
    }
}

void op_lahf(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->AH = m->cpu->flags.word & 0xff;
}

void op_stos_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    u32 addr = cpu_es(m->cpu, m->cpu->DI);
    write_memory_u8(addr, m->memory, m->cpu->AL);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 1;
        m->cpu->DI += 1;
    } else {
        m->cpu->SI -= 1;
        m->cpu->DI -= 1;
    }
}

void op_stos_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    u32 addr = cpu_es(m->cpu, m->cpu->DI);
    write_memory_u16(addr, m->memory, m->cpu->AX);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 2;
        m->cpu->DI += 2;
    } else {
        m->cpu->SI -= 2;
        m->cpu->DI -= 2;
    }
}
