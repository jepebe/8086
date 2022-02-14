#include "instructions.h"

void op_rcl_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    // overflow applies to rotate of 1 and where the carry bit and MSB are different
    u16 a = *wop.word;
    u8 n = *rop.byte & 0x1F;
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
    u8 n = *rop.byte & 0x1F;
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
    u8 n = *rop.byte & 0x1F;
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
    u8 n = *rop.byte & 0x1F;
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
    u8 n = *rop.byte & 0x1F;
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
    u8 n = *rop.byte & 0x1F;
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
    u8 n = *rop.byte & 0x1F;
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
    u8 n = *rop.byte & 0x1F;
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
