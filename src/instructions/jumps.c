#include "instructions.h"

void op_jz(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if zero (ZF = 1).
    (void) wop;
    if (m->cpu->flags.ZF == 1) {
        op_jmp_short(m, rop, wop);
    }
}

void op_ja(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if above (CF=0 and ZF=0).
    (void) wop;
    if (m->cpu->flags.ZF == 0 && m->cpu->flags.CF == 0) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jae(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if above or equal (CF=0).
    (void) wop;
    if (m->cpu->flags.CF == 0) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jb(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if below (CF=1).
    (void) wop;
    if (m->cpu->flags.CF == 1) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jbe(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if below or equal (CF=1 or ZF=1).
    (void) wop;
    if (m->cpu->flags.ZF == 1 || m->cpu->flags.CF == 1) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jnle(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if not less or equal (ZF=0 and SF=OF).
    (void) wop;
    if (m->cpu->flags.ZF == 0 && (m->cpu->flags.SF == m->cpu->flags.OF)) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jnl(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if not less (SF=OF).
    (void) wop;
    if (m->cpu->flags.SF == m->cpu->flags.OF) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jnge(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if not greater or equal (SF!=OF).
    (void) wop;
    if (m->cpu->flags.SF != m->cpu->flags.OF) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jng(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if not greater (ZF=1 or SF!=OF).
    (void) wop;
    if (m->cpu->flags.ZF == 1 || (m->cpu->flags.SF != m->cpu->flags.OF)) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jne(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if not equal (ZF=0).
    (void) wop;
    if (m->cpu->flags.ZF == 0) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jno(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if not overflow (OF=0).
    (void) wop;
    if (m->cpu->flags.OF == 0) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jnp(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if not parity (PF=0).
    (void) wop;
    if (m->cpu->flags.PF == 0) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jns(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if not sign (SF=0).
    (void) wop;
    if (m->cpu->flags.SF == 0) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jo(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if overflow (OF=1).
    (void) wop;
    if (m->cpu->flags.OF == 1) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jp(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if parity (PF=1)..
    (void) wop;
    if (m->cpu->flags.PF == 1) {
        op_jmp_short(m, rop, wop);
    }
}

void op_js(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if sign (SF=1).
    (void) wop;
    if (m->cpu->flags.SF == 1) {
        op_jmp_short(m, rop, wop);
    }
}

void op_jmp_near_indirect(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    m->cpu->IP = *rop->word;
}

void op_jmp_near_relative(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    m->cpu->IP += (s16) *rop->word;
}

void op_jmp_short(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    m->cpu->IP += (s8) *rop->byte;
}

void op_jmp_far(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    m->cpu->IP = *rop->word;
    m->cpu->CS = *(rop->word + 1);
}

void op_jmp_cxz(Machine *m, Operand *rop, Operand *wop) {
    // Jump short if CX register is 0.
    (void) wop;
    if (m->cpu->CX == 0) {
        op_jmp_short(m, rop, wop);
    }
}

void op_loop(Machine *m, Operand *rop, Operand *wop) {
    // Decrement count; jump short if count ≠ 0.
    m->cpu->CX--;
    if (m->cpu->CX != 0) {
        op_jmp_short(m, rop, wop);
    }
}

void op_loope(Machine *m, Operand *rop, Operand *wop) {
    // Decrement count; jump short if count ≠ 0 and ZF = 1.
    m->cpu->CX--;
    if (m->cpu->CX != 0 && m->cpu->flags.ZF == 1) {
        op_jmp_short(m, rop, wop);
    }
}

void op_loopne(Machine *m, Operand *rop, Operand *wop) {
    // Decrement count; jump short if count ≠ 0 and ZF = 0.
    m->cpu->CX--;
    if (m->cpu->CX != 0 && m->cpu->flags.ZF == 0) {
        op_jmp_short(m, rop, wop);
    }
}
