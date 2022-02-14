#pragma once

#include "debug.h"
#include "addressing.h"
#include "machine.h"

bool parity(u32 value);

void set_logical_flags_b(Machine *m, u8 res);

void set_logical_flags_w(Machine *m, u16 res);

u16 add_word(Machine *m, u16 a, u16 b, u8 carry_flag);

u8 add_byte(Machine *m, u8 src, u8 dest, u8 carry_flag);

u16 sub_word(Machine *m, u16 src, u16 dest, u8 borrow_flag);

u8 sub_byte(Machine *m, u8 src, u8 dest, u8 borrow_flag);

// movs
void op_les(Machine *m, ReadOperand rop, WriteOperand wop);

void op_lea(Machine *m, ReadOperand rop, WriteOperand wop);

void op_xlat(Machine *m, ReadOperand rop, WriteOperand wop);

void op_mov_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_mov_b(Machine *m, ReadOperand rop, WriteOperand wop);


// arithmetics
void op_add_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_add_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_adc_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_adc_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_sub_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_sub_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_sbb_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_sbb_b(Machine *m, ReadOperand rop, WriteOperand wop);


// bitwise
void op_and_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_and_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_or_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_or_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_xor_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_xor_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_test_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_not_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_not_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_test_b(Machine *m, ReadOperand rop, WriteOperand wop);

// jumps
void op_jmp_short(Machine *m, ReadOperand rop, WriteOperand wop);

void op_jmp_near_relative(Machine *m, ReadOperand rop, WriteOperand wop);

void op_jmp_near_indirect(Machine *m, ReadOperand rop, WriteOperand wop);

void op_jz(Machine *m, ReadOperand rop, WriteOperand wop);

void op_jmp_cxz(Machine *m, ReadOperand rop, WriteOperand wop);

/// Returns the value at the top of the stack
u16 peek_stack(Machine *machine);

void op_pushf(Machine *m, ReadOperand rop, WriteOperand wop);

void op_popf(Machine *m, ReadOperand rop, WriteOperand wop);

void op_push(Machine *m, ReadOperand rop, WriteOperand wop);

void op_pop(Machine *m, ReadOperand rop, WriteOperand wop);

void op_hlt(Machine *m, ReadOperand rop, WriteOperand wop);

void op_clc(Machine *m, ReadOperand rop, WriteOperand wop);

void op_cld(Machine *m, ReadOperand rop, WriteOperand wop);

void op_cli(Machine *m, ReadOperand rop, WriteOperand wop);

void op_cmc(Machine *m, ReadOperand rop, WriteOperand wop);

void op_stc(Machine *m, ReadOperand rop, WriteOperand wop);

void op_std(Machine *m, ReadOperand rop, WriteOperand wop);

void op_sti(Machine *m, ReadOperand rop, WriteOperand wop);

void op_nop(Machine *m, ReadOperand rop, WriteOperand wop);

void op_inc_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_inc_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_dec_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_dec_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_shl_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_shl_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_sar_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_sar_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_shr_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_shr_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_rcl_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_rcl_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_rcr_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_rcr_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_rol_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_rol_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_ror_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_ror_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_cmp_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_cmp_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_neg_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_neg_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_mul_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_mul_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_imul_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_imul_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_aad(Machine *m, ReadOperand rop, WriteOperand wop);

void op_aam(Machine *m, ReadOperand rop, WriteOperand wop);

void op_div_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_div_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_idiv_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_idiv_b(Machine *m, ReadOperand rop, WriteOperand wop);


// control
void op_int(Machine *m, ReadOperand rop, WriteOperand wop);

void op_iret(Machine *m, ReadOperand rop, WriteOperand wop);

void op_into(Machine *m, ReadOperand rop, WriteOperand wop);

void op_ret(Machine *m, ReadOperand rop, WriteOperand wop);

void op_call(Machine *m, ReadOperand rop, WriteOperand wop);

// string operations
void op_cmps_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_cmps_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_lods_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_lods_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_movs_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_movs_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_scas_w(Machine *m, ReadOperand rop, WriteOperand wop);

void op_scas_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_lahf(Machine *m, ReadOperand rop, WriteOperand wop);

void op_stos_b(Machine *m, ReadOperand rop, WriteOperand wop);

void op_stos_w(Machine *m, ReadOperand rop, WriteOperand wop);

//void op_rep(Machine *m, ReadOperand rop, WriteOperand wop);

void op_repz(Machine *m, ReadOperand rop, WriteOperand wop);

void op_repnz(Machine *m, ReadOperand rop, WriteOperand wop);

//void op_rep_ub(Machine *m, ReadOperand rop, WriteOperand wop);

// segments
void op_es(Machine *m, ReadOperand rop, WriteOperand wop);

void op_cs(Machine *m, ReadOperand rop, WriteOperand wop);

void op_ss(Machine *m, ReadOperand rop, WriteOperand wop);
