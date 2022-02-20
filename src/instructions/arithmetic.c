#include "instructions.h"

static inline u32 sign_extend_word(u16 word) {
    return ((word & 0x8000) == 0x8000) ? 0xFFFF0000 | word : word;
}

static inline u16 sign_extend_byte(u8 byte) {
    return ((byte & 0x80) == 0x80) ? 0xFF00 | byte : byte;
}

static void division_error(Machine *m) {
    (void) m;
    m->cpu->IP = m->cpu->opcode_ip; // @286 compatibility with tests

    Operand op = {
            .byte_cache = 0,
            .byte = &op.byte_cache,
    };
    op_int(m, &op, &op);

    cpu_note(m, "Division Error", 0);
}

void op_add_b(Machine *m, Operand *rop, Operand *wop) {
    *wop->byte = add_byte(m, *rop->byte, *wop->byte, 0);
}

void op_add_w(Machine *m, Operand *rop, Operand *wop) {
    *wop->word = add_word(m, *rop->word, *wop->word, 0);
}

void op_adc_b(Machine *m, Operand *rop, Operand *wop) {
    *wop->byte = add_byte(m, *rop->byte, *wop->byte, m->cpu->flags.CF);
}

void op_adc_w(Machine *m, Operand *rop, Operand *wop) {
    *wop->word = add_word(m, *rop->word, *wop->word, m->cpu->flags.CF);
}

void op_sub_w(Machine *m, Operand *rop, Operand *wop) {
    *wop->word = sub_word(m, *rop->word, *wop->word, 0);
}

void op_sub_b(Machine *m, Operand *rop, Operand *wop) {
    *wop->byte = sub_byte(m, *rop->byte, *wop->byte, 0);
}

void op_sbb_w(Machine *m, Operand *rop, Operand *wop) {
    *wop->word = sub_word(m, *rop->word, *wop->word, m->cpu->flags.CF);
}

void op_sbb_b(Machine *m, Operand *rop, Operand *wop) {
    *wop->byte = sub_byte(m, *rop->byte, *wop->byte, m->cpu->flags.CF);
}

void op_inc_b(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    uint8_t a = *wop->byte;
    u16 sum = a + 1;
    m->cpu->flags.ZF = (sum & 0xFF) == 0;
    m->cpu->flags.SF = (sum & 0x80) == 0x80;
    m->cpu->flags.AF = (((a & 0x0f) + 1) & 0x10) == 0x10;
    m->cpu->flags.PF = parity(sum);
    m->cpu->flags.OF = (a == 0x7F) ? 1 : 0;

    *wop->byte = sum & 0xFF;
}

void op_inc_w(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    u16 a = *wop->word;
    u32 sum = a + 1;
    m->cpu->flags.ZF = (sum & 0xFFFF) == 0;
    m->cpu->flags.SF = (sum & 0x8000) == 0x8000;
    m->cpu->flags.AF = (((a & 0x0f) + 1) & 0x10) == 0x10;
    m->cpu->flags.PF = parity(sum);
    m->cpu->flags.OF = (a == 0x7FFF) ? 1 : 0;
    *wop->word = sum & 0xFFFF;
}

void op_dec_w(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    u16 a = *wop->word;
    u32 sum = a - 1;
    m->cpu->flags.ZF = (sum & 0xFFFF) == 0;
    m->cpu->flags.SF = (sum & 0x8000) == 0x8000;
    m->cpu->flags.AF = 1 > (a & 0xf);
    m->cpu->flags.PF = parity(sum);
    m->cpu->flags.OF = (a == 0x8000) ? 1 : 0;
    *wop->word = sum & 0xFFFF;
}

void op_dec_b(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    u8 a = *wop->byte;
    u16 sum = a - 1;
    m->cpu->flags.ZF = (sum & 0xFF) == 0;
    m->cpu->flags.SF = (sum & 0x80) == 0x80;
    m->cpu->flags.AF = 1 > (a & 0xf);
    m->cpu->flags.PF = parity(sum);
    m->cpu->flags.OF = (a == 0x80) ? 1 : 0;
    *wop->byte = sum & 0xFF;
}

void op_cmp_w(Machine *m, Operand *rop, Operand *wop) {
    u16 src = *rop->word;
    u16 dest = *wop->word;
    sub_word(m, src, dest, 0);
}

void op_cmp_b(Machine *m, Operand *rop, Operand *wop) {
    u8 src = *rop->byte;
    u8 dest = *wop->byte;
    sub_byte(m, src, dest, 0);
}

void op_neg_w(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    *wop->word = sub_word(m, *wop->word, 0, 0);
}

void op_neg_b(Machine *m, Operand *rop, Operand *wop) {
    (void) rop;
    *wop->byte = sub_byte(m, *wop->byte, 0, 0);
}

void op_mul_w(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    u32 res = m->cpu->AX * *rop->word;
    m->cpu->DX = res >> 16;
    m->cpu->AX = res & 0x0000FFFF;

    m->cpu->flags.CF = m->cpu->DX != 0;
    m->cpu->flags.OF = m->cpu->DX != 0;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.PF = parity(res);
}

void op_mul_b(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    u16 res = m->cpu->AL * *rop->byte;
    m->cpu->AX = res;

    m->cpu->flags.CF = m->cpu->AH != 0;
    m->cpu->flags.OF = m->cpu->AH != 0;

    // these are not supposed to be set
    m->cpu->flags.ZF = res == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.PF = parity(res);
}

void op_imul_w(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    u32 src = sign_extend_word(*rop->word);
    u32 dest = sign_extend_word(m->cpu->AX);
    u32 res = dest * src;
    m->cpu->DX = res >> 16;
    m->cpu->AX = res & 0x0000FFFF;

    u32 tmp_res = sign_extend_word(m->cpu->AX);
    m->cpu->flags.CF = res != tmp_res;
    m->cpu->flags.OF = res != tmp_res;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.PF = parity(res);
}

void op_imul_b(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    u16 src = sign_extend_byte(*rop->byte);
    u16 dest = sign_extend_byte(m->cpu->AL);
    u16 res = dest * src;
    m->cpu->AH = res >> 8;
    m->cpu->AL = res & 0x00FF;

    u16 tmp_res = sign_extend_byte(m->cpu->AL);
    m->cpu->flags.CF = res != tmp_res;
    m->cpu->flags.OF = res != tmp_res;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFF) == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.PF = parity(res);
}

void op_aad(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    (void) rop;
    u8 imm8 = *rop->byte;

    m->cpu->AL += m->cpu->AH * imm8; // probably undefined behavior
    m->cpu->AH = 0;

    m->cpu->flags.ZF = (m->cpu->AL & 0xFF) == 0;
    m->cpu->flags.SF = (m->cpu->AL & 0x80) == 0x80;
    m->cpu->flags.PF = parity(m->cpu->AL);

    // these are not supposed to be set -> undefined
    m->cpu->flags.CF = 0;
    m->cpu->flags.OF = 0;
}

void op_aam(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    u8 imm8 = *rop->byte;

    if (imm8 == 0) {
        division_error(m);
        return;
    }

    u8 temp_al = m->cpu->AL;
    m->cpu->AH = temp_al / imm8; // probably undefined behavior
    m->cpu->AL = temp_al % imm8;

    m->cpu->flags.ZF = (m->cpu->AL & 0xFF) == 0;
    m->cpu->flags.SF = (m->cpu->AL & 0x80) == 0x80;
    m->cpu->flags.PF = parity(m->cpu->AL);

    // these are not supposed to be set -> undefined
    m->cpu->flags.CF = 0;
    m->cpu->flags.OF = 0;
}

void op_div_w(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    u32 src = *rop->word;

    if (src == 0x0) {
        division_error(m);
        return;
    }

    u32 dest = (m->cpu->DX << 16) | m->cpu->AX;
    u32 res = dest / src;
    if (res > 0xFFFF) {
        division_error(m);
        return;
    }
    m->cpu->AX = res & 0x0000FFFF;
    m->cpu->DX = dest % src;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.PF = parity(res);
}

void op_div_b(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    u16 src = *rop->byte;

    if (src == 0x0) {
        division_error(m);
        return;
    }

    u16 dest = m->cpu->AX;
    u16 res = dest / src;

    if (res > 0xFF) {
        division_error(m);
        return;
    }

    m->cpu->AL = res & 0x00FF;
    m->cpu->AH = dest % src;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFF) == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.PF = parity(res);
}

void op_idiv_w(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    s32 src = (s32) sign_extend_word(*rop->word);

    if (src == 0x0) {
        division_error(m);
        return;
    }

    s32 dest = (m->cpu->DX << 16) | m->cpu->AX;
    s32 res = dest / src;

    if (res < -0x7FFF || res > 0x7FFF) {
        division_error(m);
        return;
    }

    m->cpu->AX = res & 0x0000FFFF;
    m->cpu->DX = dest % src;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFFFF) == 0;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.PF = parity(res);
}

void op_idiv_b(Machine *m, Operand *rop, Operand *wop) {
    (void) wop;
    s16 src = (s16) sign_extend_byte(*rop->byte);

    if (src == 0x0) {
        division_error(m);
        return;
    }

    s16 dest = (s16) m->cpu->AX;
    // https://clang.llvm.org/extra/clang-tidy/checks/cppcoreguidelines-narrowing-conversions.html
    s16 res = dest / src;  // todo??? NOLINT(cppcoreguidelines-narrowing-conversions)

    if (res < -0x7F || res > 0x7F) {
        division_error(m);
        return;
    }

    m->cpu->AL = res & 0x00FF;
    m->cpu->AH = dest % src;

    // these are not supposed to be set
    m->cpu->flags.ZF = (res & 0xFF) == 0;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.PF = parity(res);
}
