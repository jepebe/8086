#include "instructions.h"

bool parity(u32 value) {
    u8 count = 0;
    for (int i = 0; i < 8; ++i) {
        count += (value >> i) & 0x1;
    }
    return !(count & 0x1);
}

void set_logical_flags_b(Machine *m, u8 res) {
    m->cpu->flags.PF = parity(res);
    m->cpu->flags.ZF = res == 0x00;
    m->cpu->flags.SF = (res & 0x80) == 0x80;
    m->cpu->flags.OF = 0;
}

void set_logical_flags_w(Machine *m, u16 res) {
    m->cpu->flags.PF = parity(res);
    m->cpu->flags.ZF = res == 0x0000;
    m->cpu->flags.SF = (res & 0x8000) == 0x8000;
    m->cpu->flags.OF = 0;
}

u16 add_word(Machine *m, u16 a, u16 b, u8 carry_flag) {
    u32 sum = a + b + carry_flag;
    bool carry = (sum & 0xFFFF0000) > 0;
    m->cpu->flags.CF = carry;
    m->cpu->flags.ZF = (sum & 0xFFFF) == 0;
    m->cpu->flags.SF = (sum & 0x8000) == 0x8000;
    m->cpu->flags.AF = (((a & 0x0f) + (b & 0x0f)) & 0x10) == 0x10;
    m->cpu->flags.PF = parity(sum);

    if ((a & 0x8000) == (b & 0x8000)) { // same sign
        if (a & 0x8000) { // negative
            m->cpu->flags.OF = (sum & 0xFFFF) < 0x8000;
        } else {
            m->cpu->flags.OF = sum > 0x7FFF;
        }
    } else {
        m->cpu->flags.OF = 0;
    }
    return sum & 0xFFFF;
}

u8 add_byte(Machine *m, u8 src, u8 dest, u8 carry_flag) {
    u16 sum = src + dest + carry_flag;
    m->cpu->flags.CF = (sum & 0xFF00) > 0;
    m->cpu->flags.ZF = (sum & 0xFF) == 0x00;
    m->cpu->flags.SF = (sum & 0x80) == 0x80;
    m->cpu->flags.AF = (((src & 0x0f) + (dest & 0x0f)) & 0x10) == 0x10;
    m->cpu->flags.PF = parity(sum);

    if ((src & 0x80) == (dest & 0x80)) { // same sign
        if (src & 0x80) { // negative
            m->cpu->flags.OF = (sum & 0xFF) < 0x80;
        } else {
            m->cpu->flags.OF = sum > 0x7F;
        }
    }
    return sum & 0xFF;
}

u16 sub_word(Machine *m, u16 src, u16 dest, u8 borrow_flag) {
    u32 sum = dest + (~src + 1) + (~borrow_flag + 1);
    bool borrow = (sum & 0xFFFF0000) > 0;
    m->cpu->flags.CF = borrow;
    m->cpu->flags.ZF = (sum & 0xFFFF) == 0;
    m->cpu->flags.SF = (sum & 0x8000) == 0x8000;
    m->cpu->flags.AF = ((src & 0x0f) + borrow_flag) > (dest & 0x0f);
    m->cpu->flags.PF = parity(sum);

    if ((src & 0x8000) != (dest & 0x8000)) { // not same sign
        if ((src & 0x8000) == 0x8000) { // negative
            m->cpu->flags.OF = (sum & 0xFFFF) >= 0x8000;
        } else {
            m->cpu->flags.OF = sum <= 0x7FFF;
        }
    } else {
        m->cpu->flags.OF = 0;
    }
    return sum & 0xFFFF;
}

u8 sub_byte(Machine *m, u8 src, u8 dest, u8 borrow_flag) {
    u16 sum = dest + (~src + 1) + (~borrow_flag + 1);
    bool borrow = (sum & 0xFF00) > 0;
    m->cpu->flags.CF = borrow;
    m->cpu->flags.ZF = (sum & 0xFF) == 0;
    m->cpu->flags.SF = (sum & 0x80) == 0x80;
    m->cpu->flags.AF = ((src & 0x0f) + borrow_flag) > (dest & 0x0f);
    m->cpu->flags.PF = parity(sum);

    if ((src & 0x80) != (dest & 0x80)) { // not same sign
        if ((src & 0x80) == 0x80) { // negative
            m->cpu->flags.OF = (sum & 0xFF) >= 0x80;
        } else {
            m->cpu->flags.OF = sum <= 0x7F;
        }
    } else {
        m->cpu->flags.OF = 0;
    }
    return sum & 0xFF;
}