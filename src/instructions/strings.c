#include "instructions.h"

void stos_w(Machine *m) {
    u32 addr = effective_addr(m, m->cpu->DI, ES_SEGMENT);
    write_memory_u16(addr, m->memory, m->cpu->AX);

    if (m->cpu->flags.DF == 0) {
        m->cpu->DI += 2;
    } else {
        m->cpu->DI -= 2;
    }
}

void op_stos_w(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    if (m->cpu->repeat != NO_REPEAT) {
        m->cpu->repeat = REP;
    }
    if (m->cpu->repeat == NO_REPEAT) {
        stos_w(m);
    } else {
        while (m->cpu->CX != 0) {
            stos_w(m);
            --m->cpu->CX;
        }
    }
}

void stos_b(Machine *m) {
    u32 addr = effective_addr(m, m->cpu->DI, ES_SEGMENT);
    write_memory_u8(addr, m->memory, m->cpu->AL);

    if (m->cpu->flags.DF == 0) {
        m->cpu->DI += 1;
    } else {
        m->cpu->DI -= 1;
    }
}

void op_stos_b(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    if (m->cpu->repeat != NO_REPEAT) {
        m->cpu->repeat = REP;
    }
    if (m->cpu->repeat == NO_REPEAT) {
        stos_b(m);
    } else {
        while (m->cpu->CX != 0) {
            stos_b(m);
            --m->cpu->CX;
        }
    }
}

void op_lahf(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    m->cpu->AH = m->cpu->flags.word & 0xff;
}

void scas_b(Machine *m) {
    u32 addr = effective_addr(m, m->cpu->DI, ES_SEGMENT);
    u8 src = read_memory_u8(addr, m->memory);
    u8 dest = m->cpu->AL;
    sub_byte(m, src, dest, 0);

    if (m->cpu->flags.DF == 0) {
        m->cpu->DI += 1;
    } else {
        m->cpu->DI -= 1;
    }
}

void op_scas_b(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    if (m->cpu->repeat == NO_REPEAT) {
        scas_b(m);
    } else if (m->cpu->repeat == REPZ) {
        while (m->cpu->CX != 0) {
            scas_b(m);
            --m->cpu->CX;

            if (!m->cpu->flags.ZF)
                break;
        }
    } else if (m->cpu->repeat == REPNZ) {
        while (m->cpu->CX != 0) {
            scas_b(m);
            --m->cpu->CX;

            if (m->cpu->flags.ZF)
                break;
        }
    }

}

void scas_w(Machine *m) {
    u32 addr = effective_addr(m, m->cpu->DI, ES_SEGMENT);
    u16 src = read_memory_u16(addr, m->memory);
    u16 dest = m->cpu->AX;
    sub_word(m, src, dest, 0);

    if (m->cpu->flags.DF == 0) {
        m->cpu->DI += 2;
    } else {
        m->cpu->DI -= 2;
    }
}

void op_scas_w(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    if (m->cpu->repeat == NO_REPEAT) {
        scas_w(m);
    } else if (m->cpu->repeat == REPZ) {
        while (m->cpu->CX != 0) {
            scas_w(m);
            --m->cpu->CX;

            if (!m->cpu->flags.ZF)
                break;
        }
    } else if (m->cpu->repeat == REPNZ) {
        while (m->cpu->CX != 0) {
            scas_w(m);
            --m->cpu->CX;

            if (m->cpu->flags.ZF)
                break;
        }
    }

}

void movs_b(Machine *m) {
    u32 from_addr = effective_addr(m, m->cpu->SI, DS_SEGMENT);
    u8 byte = read_memory_u8(from_addr, m->memory);
    u32 to_addr = effective_addr_no_override(m, m->cpu->DI, ES_SEGMENT);
    write_memory_u8(to_addr, m->memory, byte);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 1;
        m->cpu->DI += 1;
    } else {
        m->cpu->SI -= 1;
        m->cpu->DI -= 1;
    }
}

void op_movs_b(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    if (m->cpu->repeat != NO_REPEAT) {
        m->cpu->repeat = REP;
    }
    if (m->cpu->repeat == NO_REPEAT) {
        movs_b(m);
    } else {
        while (m->cpu->CX != 0) {
            movs_b(m);
            --m->cpu->CX;
        }
    }
}

void movs_w(Machine *m) {
    u32 from_addr = effective_addr(m, m->cpu->SI, DS_SEGMENT);
    u16 word = read_memory_u16(from_addr, m->memory);
    u32 to_addr = effective_addr_no_override(m, m->cpu->DI, ES_SEGMENT);
    write_memory_u16(to_addr, m->memory, word);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 2;
        m->cpu->DI += 2;
    } else {
        m->cpu->SI -= 2;
        m->cpu->DI -= 2;
    }
}

void op_movs_w(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    if (m->cpu->repeat != NO_REPEAT) {
        m->cpu->repeat = REP;
    }
    if (m->cpu->repeat == NO_REPEAT) {
        movs_w(m);
    } else {
        while (m->cpu->CX != 0) {
            movs_w(m);
            --m->cpu->CX;
        }
    }
}

static void lods_b(Machine *m) {
    u32 addr = effective_addr(m, m->cpu->SI, DS_SEGMENT);
    m->cpu->AL = read_memory_u8(addr, m->memory);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 1;
    } else {
        m->cpu->SI -= 1;
    }
}

void op_lods_b(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    if (m->cpu->repeat != NO_REPEAT) {
        m->cpu->repeat = REP;
    }
    if (m->cpu->repeat == NO_REPEAT) {
        lods_b(m);
    } else {
        while (m->cpu->CX != 0) {
            lods_b(m);
            --m->cpu->CX;
        }
    }
}

static void lods_w(Machine *m) {
    u32 addr = effective_addr(m, m->cpu->SI, DS_SEGMENT);
    m->cpu->AX = read_memory_u16(addr, m->memory);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 2;
    } else {
        m->cpu->SI -= 2;
    }
}

void op_lods_w(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    if (m->cpu->repeat != NO_REPEAT) {
        m->cpu->repeat = REP;
    }
    if (m->cpu->repeat == NO_REPEAT) {
        lods_w(m);
    } else {
        while (m->cpu->CX != 0) {
            lods_w(m);
            --m->cpu->CX;
        }
    }
}

static void cmps_b(Machine *m) {
    u32 src1_addr = effective_addr(m, m->cpu->SI, DS_SEGMENT);
    u8 src1 = read_memory_u8(src1_addr, m->memory);
    u32 src2_addr = effective_addr_no_override(m, m->cpu->DI, ES_SEGMENT);
    u8 src2 = read_memory_u8(src2_addr, m->memory);
    sub_byte(m, src2, src1, 0);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 1;
        m->cpu->DI += 1;
    } else {
        m->cpu->SI -= 1;
        m->cpu->DI -= 1;
    }
}

void op_cmps_b(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    if (m->cpu->repeat == NO_REPEAT) {
        cmps_b(m);
    } else if (m->cpu->repeat == REPZ) {
        while (m->cpu->CX != 0) {
            cmps_b(m);
            --m->cpu->CX;

            if (!m->cpu->flags.ZF)
                break;
        }
    } else if (m->cpu->repeat == REPNZ) {
        while (m->cpu->CX != 0) {
            cmps_b(m);
            --m->cpu->CX;

            if (m->cpu->flags.ZF)
                break;
        }
    }
}

static void cmps_w(Machine *m) {
    u32 src1_addr = effective_addr(m, m->cpu->SI, DS_SEGMENT);
    u16 src1 = read_memory_u16(src1_addr, m->memory);
    u32 src2_addr = effective_addr_no_override(m, m->cpu->DI, ES_SEGMENT);
    u16 src2 = read_memory_u16(src2_addr, m->memory);
    sub_word(m, src2, src1, 0);

    if (m->cpu->flags.DF == 0) {
        m->cpu->SI += 2;
        m->cpu->DI += 2;
    } else {
        m->cpu->SI -= 2;
        m->cpu->DI -= 2;
    }
}

void op_cmps_w(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    if (m->cpu->repeat == NO_REPEAT) {
        cmps_w(m);
    } else if (m->cpu->repeat == REPZ) {
        while (m->cpu->CX != 0) {
            cmps_w(m);
            --m->cpu->CX;

            if (!m->cpu->flags.ZF)
                break;
        }
    } else if (m->cpu->repeat == REPNZ) {
        while (m->cpu->CX != 0) {
            cmps_w(m);
            --m->cpu->CX;

            if (m->cpu->flags.ZF)
                break;
        }
    }
}

void op_repz(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    (void) m;
    m->cpu->repeat = REPZ;
}

void op_repnz(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    (void) wop;
    (void) m;
    m->cpu->repeat = REPNZ;
}
