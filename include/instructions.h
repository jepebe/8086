#pragma once

#include "cpu.h"

void op_mov_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) cpu;
    *wop.word = rop.word;
}

void op_mov_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) cpu;
    *wop.byte = rop.byte;
}


void op_add_b(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) cpu;
    u16 sum = rop.byte + *wop.byte;
    bool carry = (sum & 0xFF00) > 0;
    cpu->CF = carry;
    cpu->OF = carry;
    sum = sum & 0xFF;
    cpu->ZF = sum == 0;
    *wop.byte = sum;
}

void op_add_w(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) cpu;
    u32 sum = rop.word + *wop.word;
    bool carry = (sum & 0xFFFF0000) > 0;
    cpu->CF = carry;
    cpu->OF = carry;
    sum = sum & 0xFFFF;
    cpu->ZF = sum == 0;
    *wop.word = sum;
}

void op_jmp(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    cpu->IP += (s8) rop.byte;
}

void op_pushf(CPU *cpu, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    *wop.word = cpu->Flags;
    cpu->SP -= 2;
}
