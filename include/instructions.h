#pragma once

#include <stddef.h>
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
void op_les(Machine *m, Operand *rop, Operand *wop);

void op_lds(Machine *m, Operand *rop, Operand *wop);

void op_lea(Machine *m, Operand *rop, Operand *wop);

void op_xlat(Machine *m, Operand *rop, Operand *wop);

void op_mov_w(Machine *m, Operand *rop, Operand *wop);

void op_mov_b(Machine *m, Operand *rop, Operand *wop);

void op_xchg_b(Machine *m, Operand *rop, Operand *wop);

void op_xchg_w(Machine *m, Operand *rop, Operand *wop);

void op_sahf(Machine *m, Operand *rop, Operand *wop);

void op_lahf(Machine *m, Operand *rop, Operand *wop);


// arithmetics
void op_add_b(Machine *m, Operand *rop, Operand *wop);

void op_add_w(Machine *m, Operand *rop, Operand *wop);

void op_adc_b(Machine *m, Operand *rop, Operand *wop);

void op_adc_w(Machine *m, Operand *rop, Operand *wop);

void op_sub_w(Machine *m, Operand *rop, Operand *wop);

void op_sub_b(Machine *m, Operand *rop, Operand *wop);

void op_sbb_w(Machine *m, Operand *rop, Operand *wop);

void op_sbb_b(Machine *m, Operand *rop, Operand *wop);

void op_inc_b(Machine *m, Operand *rop, Operand *wop);

void op_inc_w(Machine *m, Operand *rop, Operand *wop);

void op_dec_w(Machine *m, Operand *rop, Operand *wop);

void op_dec_b(Machine *m, Operand *rop, Operand *wop);


// bitwise
void op_and_w(Machine *m, Operand *rop, Operand *wop);

void op_and_b(Machine *m, Operand *rop, Operand *wop);

void op_or_w(Machine *m, Operand *rop, Operand *wop);

void op_or_b(Machine *m, Operand *rop, Operand *wop);

void op_xor_w(Machine *m, Operand *rop, Operand *wop);

void op_xor_b(Machine *m, Operand *rop, Operand *wop);

void op_test_w(Machine *m, Operand *rop, Operand *wop);

void op_not_w(Machine *m, Operand *rop, Operand *wop);

void op_not_b(Machine *m, Operand *rop, Operand *wop);

void op_test_b(Machine *m, Operand *rop, Operand *wop);

// jumps
void op_jmp_short(Machine *m, Operand *rop, Operand *wop);

void op_jmp_far(Machine *m, Operand *rop, Operand *wop);

void op_jmp_near_relative(Machine *m, Operand *rop, Operand *wop);

void op_jmp_near_indirect(Machine *m, Operand *rop, Operand *wop);

void op_jz(Machine *m, Operand *rop, Operand *wop);

void op_ja(Machine *m, Operand *rop, Operand *wop);

void op_jae(Machine *m, Operand *rop, Operand *wop);

void op_jb(Machine *m, Operand *rop, Operand *wop);

void op_jbe(Machine *m, Operand *rop, Operand *wop);

void op_jnle(Machine *m, Operand *rop, Operand *wop);

void op_jnl(Machine *m, Operand *rop, Operand *wop);

void op_jnge(Machine *m, Operand *rop, Operand *wop);

void op_jng(Machine *m, Operand *rop, Operand *wop);

void op_jne(Machine *m, Operand *rop, Operand *wop);

void op_jno(Machine *m, Operand *rop, Operand *wop);

void op_jnp(Machine *m, Operand *rop, Operand *wop);

void op_jns(Machine *m, Operand *rop, Operand *wop);

void op_jo(Machine *m, Operand *rop, Operand *wop);

void op_jp(Machine *m, Operand *rop, Operand *wop);

void op_js(Machine *m, Operand *rop, Operand *wop);

void op_loop(Machine *m, Operand *rop, Operand *wop);

void op_loope(Machine *m, Operand *rop, Operand *wop);

void op_loopne(Machine *m, Operand *rop, Operand *wop);

void op_jmp_cxz(Machine *m, Operand *rop, Operand *wop);


/// Returns the value at the top of the stack
u16 peek_stack(Machine *machine);

void op_pushf(Machine *m, Operand *rop, Operand *wop);

void op_popf(Machine *m, Operand *rop, Operand *wop);

void op_push(Machine *m, Operand *rop, Operand *wop);

void op_pop(Machine *m, Operand *rop, Operand *wop);


// control

/// Halt
void op_hlt(Machine *m, Operand *rop, Operand *wop);

/// Clear CF flag.
void op_clc(Machine *m, Operand *rop, Operand *wop);

/// Clear DF flag.
void op_cld(Machine *m, Operand *rop, Operand *wop);

/// Clear interrupt flag; interrupts disabled when interrupt flag cleared.
void op_cli(Machine *m, Operand *rop, Operand *wop);

/// Complement CF flag.
void op_cmc(Machine *m, Operand *rop, Operand *wop);

void op_stc(Machine *m, Operand *rop, Operand *wop);

void op_std(Machine *m, Operand *rop, Operand *wop);

void op_sti(Machine *m, Operand *rop, Operand *wop);

// One byte no-operation instruction.
void op_nop(Machine *m, Operand *rop, Operand *wop);

// segments
void op_es(Machine *m, Operand *rop, Operand *wop);

void op_cs(Machine *m, Operand *rop, Operand *wop);

void op_ss(Machine *m, Operand *rop, Operand *wop);


void op_shl_w(Machine *m, Operand *rop, Operand *wop);

void op_shl_b(Machine *m, Operand *rop, Operand *wop);

void op_sar_w(Machine *m, Operand *rop, Operand *wop);

void op_sar_b(Machine *m, Operand *rop, Operand *wop);

void op_shr_w(Machine *m, Operand *rop, Operand *wop);

void op_shr_b(Machine *m, Operand *rop, Operand *wop);

void op_rcl_w(Machine *m, Operand *rop, Operand *wop);

void op_rcl_b(Machine *m, Operand *rop, Operand *wop);

void op_rcr_w(Machine *m, Operand *rop, Operand *wop);

void op_rcr_b(Machine *m, Operand *rop, Operand *wop);

void op_rol_w(Machine *m, Operand *rop, Operand *wop);

void op_rol_b(Machine *m, Operand *rop, Operand *wop);

void op_ror_w(Machine *m, Operand *rop, Operand *wop);

void op_ror_b(Machine *m, Operand *rop, Operand *wop);

void op_cmp_w(Machine *m, Operand *rop, Operand *wop);

void op_cmp_b(Machine *m, Operand *rop, Operand *wop);

void op_neg_w(Machine *m, Operand *rop, Operand *wop);

void op_neg_b(Machine *m, Operand *rop, Operand *wop);

void op_mul_w(Machine *m, Operand *rop, Operand *wop);

void op_mul_b(Machine *m, Operand *rop, Operand *wop);

void op_imul_w(Machine *m, Operand *rop, Operand *wop);

void op_imul_b(Machine *m, Operand *rop, Operand *wop);

void op_aad(Machine *m, Operand *rop, Operand *wop);

void op_aam(Machine *m, Operand *rop, Operand *wop);

void op_div_w(Machine *m, Operand *rop, Operand *wop);

void op_div_b(Machine *m, Operand *rop, Operand *wop);

void op_idiv_w(Machine *m, Operand *rop, Operand *wop);

void op_idiv_b(Machine *m, Operand *rop, Operand *wop);


// control
void op_int(Machine *m, Operand *rop, Operand *wop);

void op_iret(Machine *m, Operand *rop, Operand *wop);

void op_into(Machine *m, Operand *rop, Operand *wop);

void op_ret(Machine *m, Operand *rop, Operand *wop);

void op_retn(Machine *m, Operand *rop, Operand *wop);

void op_call_direct(Machine *m, Operand *rop, Operand *wop);

void op_call_relative(Machine *m, Operand *rop, Operand *wop);

void op_call_far(Machine *m, Operand *rop, Operand *wop);

void op_ret_far(Machine *m, Operand *rop, Operand *wop);

void op_retn_far(Machine *m, Operand *rop, Operand *wop);


// string operations
void op_cmps_w(Machine *m, Operand *rop, Operand *wop);

void op_cmps_b(Machine *m, Operand *rop, Operand *wop);

void op_lods_w(Machine *m, Operand *rop, Operand *wop);

void op_lods_b(Machine *m, Operand *rop, Operand *wop);

void op_movs_w(Machine *m, Operand *rop, Operand *wop);

void op_movs_b(Machine *m, Operand *rop, Operand *wop);

void op_scas_w(Machine *m, Operand *rop, Operand *wop);

void op_scas_b(Machine *m, Operand *rop, Operand *wop);

void op_stos_b(Machine *m, Operand *rop, Operand *wop);

void op_stos_w(Machine *m, Operand *rop, Operand *wop);


void op_repz(Machine *m, Operand *rop, Operand *wop);

void op_repnz(Machine *m, Operand *rop, Operand *wop);


// io
void op_out_w(Machine *m, Operand *rop, Operand *wop);

// BCD and CNV

/// ASCII adjust AL after addition.
void op_aaa(Machine *m, Operand *rop, Operand *wop);

/// ASCII adjust AL after subtraction.
void op_aas(Machine *m, Operand *rop, Operand *wop);

/// Decimal adjust AL after addition.
void op_daa(Machine *m, Operand *rop, Operand *wop);

/// Decimal adjust AL after subtraction.
void op_das(Machine *m, Operand *rop, Operand *wop);

/// AX ← sign-extend of AL.
void op_cbw(Machine *m, Operand *rop, Operand *wop);

/// DX:AX ← sign-extend of AX.
void op_cwd(Machine *m, Operand *rop, Operand *wop);
