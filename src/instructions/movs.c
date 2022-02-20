#include "instructions.h"

void op_mov_w(Machine *m, Operand *rop, Operand *wop) {
    (void) m;
    *wop->word = *rop->word;
}

void op_mov_b(Machine *m, Operand *rop, Operand *wop) {
    (void) m;
    *wop->byte = *rop->byte;
}

void op_les(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    (void) m;

    SegmentOverride segment = m->cpu->segment_override;
    if(segment == DEFAULT_SEGMENT) {
        segment = ES_SEGMENT;
    }

    *wop->word = *rop->word; // offset

    switch(segment) {
        case CS_SEGMENT:
            m->cpu->CS = *(rop->word + 1);
            break;
        case DS_SEGMENT:
            m->cpu->DS = *(rop->word + 1);
            break;
        case ES_SEGMENT:
            m->cpu->ES = *(rop->word + 1);
            break;
        case SS_SEGMENT:
            m->cpu->SS = *(rop->word + 1);
            break;
        default:
            break;
    }
}

void op_lea(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    (void) m;
    (*wop->word) = rop->addr & 0xFFFF;
}

void op_xlat(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    u8 idx = m->cpu->AL;
    u32 addr = effective_addr(m, m->cpu->BX + idx, DS_SEGMENT);
    m->cpu->AL = read_memory_u8(addr, m->memory);
}
