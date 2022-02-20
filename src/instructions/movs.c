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

void op_lds(Machine *m, Operand *rop, Operand *wop) {
    // Load DS:r16 with far pointer from memory.
    (void) m;

    *wop->word = *rop->word; // offset
    m->cpu->DS = *(rop->word + 1); // segment
}

void op_lea(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    (void) m;
    (*wop->word) = rop->word_cache & 0xFFFF;
}

void op_xlat(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    u8 idx = m->cpu->AL;
    u32 addr = effective_addr(m, m->cpu->BX + idx, DS_SEGMENT);
    m->cpu->AL = read_memory_u8(addr, m->memory);
}

void op_xchg_b(Machine *m, Operand *rop, Operand *wop) {
    // Exchange r8 with memory.
    // fixme: can the reg/mem order be reversed? How?
    (void) m;
    u8 temp = *wop->byte;
    *wop->byte = *rop->byte;
    *rop->byte = temp;
}

void op_xchg_w(Machine *m, Operand *rop, Operand *wop) {
    // Exchange r16 with AX or memory.
    // fixme: can the reg/mem order be reversed? How?
    (void) m;
    u16 temp = *wop->word;
    *wop->word = *rop->word;
    *rop->word = temp;
}

void op_sahf(Machine *m, Operand *rop, Operand *wop) {
    // Loads SF, ZF, AF, PF, and CF from AH into EFLAGS register.
    (void) rop;
    (void) wop;
    u8 ah = m->cpu->AH;
    // magic number 0xD5 and 2 -> clear unused bits and set 2 bit to 1
    m->cpu->flags.word = (m->cpu->flags.word & 0xFF00) | ((ah & 0xD5) | 0x2);
}

void op_lahf(Machine *m, Operand *rop, Operand *wop) {
    // Load: AH ← EFLAGS(SF:ZF:0:AF:0:PF:1:CF).
    (void) rop;
    (void) wop;
    m->cpu->AH = m->cpu->flags.word & 0xff;
}
