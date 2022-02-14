#include "instructions.h"

void op_jz(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    if (m->cpu->flags.ZF == 1) {
        m->cpu->IP += (s8) *rop.byte;
    }
}

void op_jmp_near_indirect(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    m->cpu->IP = *rop.word;
}

void op_jmp_near_relative(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    m->cpu->IP += (s16) *rop.word;
}

void op_jmp_short(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    m->cpu->IP += (s8) *rop.byte;
}

void op_jmp_cxz(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    if (m->cpu->CX == 0) {
        m->cpu->IP += (s8) *rop.byte;
    }
}
