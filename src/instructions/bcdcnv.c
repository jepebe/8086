#include "instructions.h"

void op_aaa(Machine *m, Operand *rop, Operand *wop) {
    //ASCII adjust AL after addition.
    (void) rop;
    (void) wop;
    if (((m->cpu->AL & 0x0F) > 9) || m->cpu->flags.AF == 1) {
        m->cpu->AX += 0x0106;
        m->cpu->flags.AF = 1;
        m->cpu->flags.CF = 1;
    } else {
        m->cpu->flags.AF = 0;
        m->cpu->flags.CF = 0;
    }
    m->cpu->AL = m->cpu->AL & 0x0F;

    // undefined
    m->cpu->flags.PF = parity(m->cpu->AL);
    m->cpu->flags.ZF = m->cpu->AL == 0x00;
}

void op_aas(Machine *m, Operand *rop, Operand *wop) {
    //ASCII adjust AL after subtraction.
    (void) rop;
    (void) wop;
    if (((m->cpu->AL & 0x0F) > 9) || m->cpu->flags.AF == 1) {
        m->cpu->AX -= 0x06;
        m->cpu->AH -= 0x01;
        m->cpu->flags.AF = 1;
        m->cpu->flags.CF = 1;
    } else {
        m->cpu->flags.AF = 0;
        m->cpu->flags.CF = 0;
    }
    m->cpu->AL = m->cpu->AL & 0x0F;

    // undefined
    m->cpu->flags.PF = parity(m->cpu->AL);
    m->cpu->flags.ZF = m->cpu->AL == 0x00;
}

void op_daa(Machine *m, Operand *rop, Operand *wop) {
    // Decimal adjust AL after addition.
    (void) rop;
    (void) wop;
    u8 old_al = m->cpu->AL;
    bool old_cf = m->cpu->flags.CF;
    m->cpu->flags.CF = 0;

    if (((m->cpu->AL & 0x0F) > 9) || m->cpu->flags.AF == 1) {
        m->cpu->AL += 0x06;
        m->cpu->flags.CF = old_cf || ((m->cpu->AL & 0x0F) < (old_al & 0x0F));
        m->cpu->flags.AF = 1;
    } else {
        m->cpu->flags.AF = 0;
    }

    if ((old_al > 0x99) || (old_cf == 1)) {
        m->cpu->AL += 0x60;
        m->cpu->flags.CF = 1;
    } else {
        m->cpu->flags.CF = 0;
    }

    m->cpu->flags.PF = parity(m->cpu->AL);
    m->cpu->flags.ZF = m->cpu->AL == 0x00;
    m->cpu->flags.SF = (m->cpu->AL & 0x80) == 0x80;
}

void op_das(Machine *m, Operand *rop, Operand *wop) {
    // Decimal adjust AL after subtraction.
    (void) rop;
    (void) wop;
    u8 old_al = m->cpu->AL;
    bool old_cf = m->cpu->flags.CF;
    m->cpu->flags.CF = 0;

    if (((m->cpu->AL & 0x0F) > 9) || m->cpu->flags.AF == 1) {
        m->cpu->AL -= 0x06;
        m->cpu->flags.CF = old_cf || ((m->cpu->AL & 0x0F) > (old_al & 0x0F));
        m->cpu->flags.AF = 1;
    } else {
        m->cpu->flags.AF = 0;
    }

    if ((old_al > 0x99) || (old_cf == 1)) {
        m->cpu->AL -= 0x60;
        m->cpu->flags.CF = 1;
    } else {
        m->cpu->flags.CF = 0;
    }

    m->cpu->flags.PF = parity(m->cpu->AL);
    m->cpu->flags.ZF = m->cpu->AL == 0x00;
    m->cpu->flags.SF = (m->cpu->AL & 0x80) == 0x80;
}

void op_cbw(Machine *m, Operand *rop, Operand *wop) {
    // AX ← sign-extend of AL.
    (void) rop;
    (void) wop;
    m->cpu->AX = ((m->cpu->AL & 0x80) == 0x80) ? 0xFF00 | m->cpu->AL : m->cpu->AL;
}

void op_cwd(Machine *m, Operand *rop, Operand *wop) {
    // DX:AX ← sign-extend of AX.
    (void) rop;
    (void) wop;
    m->cpu->DX = (m->cpu->AX & 0x8000) == 0x8000 ? 0xFFFF : 0x0000;
}
