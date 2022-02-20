#include "instructions.h"

void op_shl_w(Machine *m, Operand *rop, Operand *wop) {
    // overflow applies to shift of 1 and where the carry bit and MSB are different
    u16 a = *wop->word;
    u8 n = *rop->word;
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
    *wop->word = res;
}

void op_shl_b(Machine *m, Operand *rop, Operand *wop) {
    // overflow applies to shift of 1 and where the carry bit and MSB are different
    u8 a = *wop->byte;
    u8 n = *rop->byte;
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
    *wop->byte = res;
}

void op_sar_w(Machine *m, Operand *rop, Operand *wop) {
    // Need to do sign extension on SAR
    // Overflow is cleared on shift by 1
    u16 a = *wop->word;
    u8 n = *rop->word;
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
    *wop->word = res;
}

void op_sar_b(Machine *m, Operand *rop, Operand *wop) {
    // Need to do sign extension on SAR
    // Overflow is cleared on shift by 1
    u8 a = *wop->byte;
    u8 n = *rop->byte;
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
    *wop->byte = res;
}

void op_shr_w(Machine *m, Operand *rop, Operand *wop) {
    // Overflow is set to MSB of operand
    u16 a = *wop->word;
    u8 n = *rop->word;
    if (n == 0) {
        return;
    }
    u16 res = a >> n;

    m->cpu->flags.CF = (a >> (n - 1)) & 0x1;
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.OF = (a & 0x8000) == 0x8000;
    m->cpu->flags.PF = parity(res);
    *wop->word = res;
}

void op_shr_b(Machine *m, Operand *rop, Operand *wop) {
    // Overflow is set to MSB of operand
    u8 a = *wop->byte;
    u8 n = *rop->byte;
    if (n == 0) {
        return;
    }
    u8 res = a >> n;

    m->cpu->flags.CF = (a >> (n - 1)) & 0x1;
    m->cpu->flags.ZF = (res & 0xFF) == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.OF = (a & 0x80) == 0x80;
    m->cpu->flags.PF = parity(res);
    *wop->byte = res;
}
