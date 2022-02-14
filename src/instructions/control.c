#include "instructions.h"

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

void op_hlt(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->halted = true;
}

void op_ret(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    op_pop(m, (ReadOperand) {0}, (WriteOperand) {.word=&m->cpu->IP});
}

void op_iret(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    op_pop(m, (ReadOperand) {0}, (WriteOperand) {.word=&m->cpu->IP});
    op_pop(m, (ReadOperand) {0}, (WriteOperand) {.word=&m->cpu->CS});
    op_popf(m, (ReadOperand) {0}, (WriteOperand) {0});
}

void op_call(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    op_push(m, (ReadOperand) {.word = &m->cpu->IP}, (WriteOperand) {0});
    m->cpu->IP = *(rop.word);
}

void op_into(Machine *m, ReadOperand rop, WriteOperand wop) {
    // interrupt on overflow
    (void) rop;
    (void) wop;
    u8 temp = 4;
    if (m->cpu->flags.OF) {
        op_int(m, (ReadOperand) {.byte = &temp}, (WriteOperand) {0});
    }
}

void op_int(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    cpu_note_int(m, "Interrupt %d", *rop.byte);
    u8 interrupt = *rop.byte;

    op_pushf(m, (ReadOperand) {0}, (WriteOperand) {0});
    m->cpu->flags.IF = 0;
    m->cpu->flags.TF = 0;
    op_push(m, (ReadOperand) {.word = &m->cpu->CS}, (WriteOperand) {0});
    op_push(m, (ReadOperand) {.word = &m->cpu->IP}, (WriteOperand) {0});

    m->cpu->IP = read_memory_u16(interrupt * 4, m->memory);
    m->cpu->CS = read_memory_u16(interrupt * 4 + 2, m->memory);
}

void op_es(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    (void) m;
    m->cpu->segment_override = ES_SEGMENT;
}

void op_cs(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    (void) m;
    m->cpu->segment_override = CS_SEGMENT;
}

void op_ss(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    (void) m;
    m->cpu->segment_override = SS_SEGMENT;
}
