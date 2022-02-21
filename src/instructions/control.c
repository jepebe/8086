#include "instructions.h"

void op_clc(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.CF = 0;
}

void op_cld(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.DF = 0;
}

void op_cli(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.IF = 0;
}

void op_cmc(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.CF = !m->cpu->flags.CF;
}

void op_stc(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.CF = 1;
}

void op_std(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.DF = 1;
}

void op_sti(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    m->cpu->flags.IF = 1;
}

void op_nop(Machine *m, Operand *rop, Operand *wop) {
    (void) m;
    (void) rop;
    (void) wop;
}

void op_hlt(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    m->cpu->halted = true;
}

void op_ret(Machine *m, Operand *rop, Operand *wop) {
    // Near return to calling procedure.
    (void) rop;
    (void) wop;
    op_pop(m, NULL, &m->IP);
}

void op_retn(Machine *m, Operand *rop, Operand *wop) {
    // Near return to calling procedure and pop imm16 bytes from stack.
    (void) rop;
    (void) wop;
    op_pop(m, NULL, &m->IP);

    m->cpu->SP += *rop->word; // pop imm16 number of bytes from the stack
}

void op_ret_far(Machine *m, Operand *rop, Operand *wop) {
    // Far return to calling procedure.
    (void) wop;
    (void) rop;
    op_pop(m, NULL, &m->IP);
    op_pop(m, NULL, &m->CS);
}

void op_retn_far(Machine *m, Operand *rop, Operand *wop) {
    // Far return to calling procedure and pop imm16 bytes from stack.
    (void) wop;
    op_pop(m, NULL, &m->IP);
    op_pop(m, NULL, &m->CS);

    m->cpu->SP += *rop->word; // pop imm16 number of bytes from the stack
}

void op_iret(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    op_pop(m, NULL, &m->IP);
    op_pop(m, NULL, &m->CS);
    op_popf(m, NULL, NULL);
}

void op_call_direct(Machine *m, Operand *rop, Operand *wop) {
    // Call near, absolute indirect, address given in r/m16.
    (void) wop;
    op_push(m, &m->IP, NULL);
    m->cpu->IP = *(rop->word);
}

void op_call_relative(Machine *m, Operand *rop, Operand *wop) {
    // Call near, relative, displacement relative to next instruction.
    (void) wop;
    dump_cpu(m);
    op_push(m, &m->IP, NULL);
    m->cpu->IP += (s16) *(rop->word);
    dump_cpu(m);
}

void op_call_far(Machine *m, Operand *rop, Operand *wop) {
    // Call far, absolute, address given in operand.
    (void) wop;
    op_push(m, &m->CS, NULL);
    op_push(m, &m->IP, NULL);
    m->cpu->IP = *rop->word;
    m->cpu->CS = *(rop->word + 1);
}

void op_into(Machine *m, Operand *rop, Operand *wop) {
    // interrupt on overflow
    (void) rop;
    (void) wop;
    if (m->cpu->flags.OF) {
        Operand op = {
                .byte_cache = 4,
                .byte = &op.byte_cache
        };
        op_int(m, &op, NULL);
    }
}

void op_int(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    u8 interrupt = *rop->byte;

    op_pushf(m, NULL, NULL);
    m->cpu->flags.IF = 0;
    m->cpu->flags.TF = 0;
    op_push(m, &m->CS, NULL);
    op_push(m, &m->IP, NULL);

    m->cpu->IP = read_memory_u16(interrupt * 4, m->memory);
    m->cpu->CS = read_memory_u16(interrupt * 4 + 2, m->memory);
}

void op_es(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    (void) m;
    m->cpu->segment_override = ES_SEGMENT;
}

void op_cs(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    (void) m;
    m->cpu->segment_override = CS_SEGMENT;
}

void op_ss(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    (void) m;
    m->cpu->segment_override = SS_SEGMENT;
}
