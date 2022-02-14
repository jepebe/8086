#include "instructions.h"

void op_and_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    u16 src = *rop.word;
    u16 dest = *wop.word;
    u16 res = dest & src;

    set_logical_flags_w(m, res);
    *wop.word = res;
}

void op_and_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    u8 src = *rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest & src;

    set_logical_flags_b(m, res);
    *wop.byte = res;
}

void op_or_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    u16 src = *rop.word;
    u16 dest = *wop.word;
    u16 res = dest | src;

    set_logical_flags_w(m, res);
    *wop.word = res;
}

void op_or_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    u8 src = *rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest | src;

    set_logical_flags_b(m, res);
    *wop.byte = res;
}

void op_xor_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    u16 src = *rop.word;
    u16 dest = *wop.word;
    u16 res = dest ^ src;

    set_logical_flags_w(m, res);
    *wop.word = res;
}

void op_xor_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    u8 src = *rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest ^ src;

    set_logical_flags_b(m, res);
    *wop.byte = res;
}

void op_test_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    u8 src = *rop.byte;
    u8 dest = *wop.byte;
    u8 res = dest & src;

    set_logical_flags_b(m, res);
}

void op_test_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    u16 src = *rop.word;
    u16 dest = *wop.word;
    u16 res = dest & src;

    set_logical_flags_w(m, res);
}

void op_not_b(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) m;
    (void) rop;
    *wop.byte = ~(*wop.byte);
}

void op_not_w(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) m;
    (void) rop;
    *wop.word = ~(*wop.word);
}
