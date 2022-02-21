#include "opcodes.h"
#include "machine.h"
#include "debug.h"
#include "instructions.h"

const Opcode opcodes[] = {
        [0x00] = {op_add_b, RMB, RB, "ADD"},
        [0x01] = {op_add_w, RMW, RW, "ADD"},
        [0x02] = {op_add_b, RB, RMB, "ADD"},
        [0x03] = {op_add_w, RW, RMW, "ADD"},
        [0x04] = {op_add_b, R_AL, IB, "ADD"},
        [0x05] = {op_add_w, R_AX, IW, "ADD"},
        [0x06] = {op_push, Implied, R_ES, "PUSH"},
        [0x07] = {op_pop, R_ES, Implied, "POP"},
        [0x08] = {op_or_b, RMB, RB, "OR"},
        [0x09] = {op_or_w, RMW, RW, "OR"},
        [0x0A] = {op_or_b, RB, RMB, "OR"},
        [0x0B] = {op_or_w, RW, RMW, "OR"},
        [0x0C] = {op_or_b, R_AL, IB, "OR"},
        [0x0D] = {op_or_w, R_AX, IW, "OR"},

        [0x10] = {op_adc_b, RMB, RB, "ADC"},
        [0x11] = {op_adc_w, RMW, RW, "ADC"},
        [0x12] = {op_adc_b, RB, RMB, "ADC"},
        [0x13] = {op_adc_w, RW, RMW, "ADC"},
        [0x14] = {op_adc_b, R_AL, IB, "ADC"},
        [0x15] = {op_adc_w, R_AX, IW, "ADC"},
        [0x18] = {op_sbb_b, RMB, RB, "SBB"},
        [0x19] = {op_sbb_w, RMW, RW, "SBB"},
        [0x1A] = {op_sbb_b, RB, RMB, "SBB"},
        [0x1B] = {op_sbb_w, RW, RMW, "SBB"},
        [0x1C] = {op_sbb_b, R_AL, IB, "SBB"},

        [0x20] = {op_and_b, RMB, RB, "AND"},
        [0x21] = {op_and_w, RMW, RW, "AND"},
        [0x22] = {op_and_b, RB, RMB, "AND"},
        [0x23] = {op_and_w, RW, RMW, "AND"},
        [0x24] = {op_and_b, R_AL, IB, "AND"},
        [0x25] = {op_and_w, R_AX, IW, "AND"},
        [0x26] = {op_es, Implied, Implied, "ES:"},
        [0x27] = {op_daa, Implied, Implied, "DAA"},
        [0x28] = {op_sub_b, RMB, RB, "SUB"},
        [0x29] = {op_sub_w, RMW, RW, "SUB"},
        [0x2A] = {op_sub_b, RB, RMB, "SUB"},
        [0x2B] = {op_sub_w, RW, RMW, "SUB"},
        [0x2C] = {op_sub_b, R_AL, IB, "SUB"},
        [0x2E] = {op_cs, Implied, Implied, "CS:"},
        [0x2F] = {op_das, Implied, Implied, "DAS"},

        [0x30] = {op_xor_b, RMB, RB, "XOR"},
        [0x31] = {op_xor_w, RMW, RW, "XOR"},
        [0x32] = {op_xor_b, RB, RMB, "XOR"},
        [0x33] = {op_xor_w, RW, RMW, "XOR"},
        [0x34] = {op_xor_b, R_AL, IB, "XOR"},
        [0x35] = {op_xor_w, R_AX, IW, "XOR"},
        [0x36] = {op_ss, Implied, Implied, "SS:"},
        [0x37] = {op_aaa, Implied, Implied, "AAA"},
        [0x38] = {op_cmp_b, RMB, RB, "CMP"},
        [0x39] = {op_cmp_w, RMW, RW, "CMP"},
        [0x3A] = {op_cmp_b, RB, RMB, "CMP"},
        [0x3C] = {op_cmp_b, R_AL, IB, "CMP"},
        [0x3F] = {op_aas, Implied, Implied, "AAS"},

        [0x47] = {op_inc_w, R_DI, Implied, "INC"},
        [0x4F] = {op_dec_w, R_DI, Implied, "DEC"},

        [0x50] = {op_push, Implied, R_AX, "PUSH"},
        [0x51] = {op_push, Implied, R_CX, "PUSH"},
        [0x52] = {op_push, Implied, R_DX, "PUSH"},
        [0x53] = {op_push, Implied, R_BX, "PUSH"},
        [0x57] = {op_push, Implied, R_DI, "PUSH"},
        [0x58] = {op_pop, R_AX, Implied, "POP"},
        [0x59] = {op_pop, R_CX, Implied, "POP"},
        [0x5A] = {op_pop, R_DX, Implied, "POP"},
        [0x5B] = {op_pop, R_BX, Implied, "POP"},
        [0x5F] = {op_pop, R_DI, Implied, "POP"},

        [0x70] = {op_jo, Implied, IB, "JO"},
        [0x71] = {op_jno, Implied, IB, "JNO"},
        [0x72] = {op_jb, Implied, IB, "JB"},
        [0x73] = {op_jae, Implied, IB, "JAE"},
        [0x74] = {op_jz, Implied, IB, "JZ"}, // JE
        [0x75] = {op_jne, Implied, IB, "JNE"},
        [0x76] = {op_jbe, Implied, IB, "JBE"},
        [0x77] = {op_ja, Implied, IB, "JA"},
        [0x78] = {op_js, Implied, IB, "JS"},
        [0x79] = {op_jns, Implied, IB, "JNS"},
        [0x7A] = {op_jp, Implied, IB, "JP"},
        [0x7B] = {op_jnp, Implied, IB, "JNP"},
        [0x7C] = {op_jnge, Implied, IB, "JNGE"},
        [0x7D] = {op_jnl, Implied, IB, "JNL"},
        [0x7E] = {op_jng, Implied, IB, "JNG"},
        [0x7F] = {op_jnle, Implied, IB, "JNLE"}, // JG

        [0x84] = {op_test_b, RMB, RB, "TEST"},
        [0x85] = {op_test_w, RMW, RW, "TEST"},
        [0x86] = {op_xchg_b, RB, RMB, "XCHG"},
        [0x87] = {op_xchg_w, RW, RMW, "XCHG"},
        [0x88] = {op_mov_b, RMB, RB, "MOV"},
        [0x89] = {op_mov_w, RMW, RW, "MOV"},
        [0x8A] = {op_mov_b, RB, RMB, "MOV"},
        [0x8B] = {op_mov_w, RW, RMW, "MOV"},
        [0x8C] = {op_mov_w, RMW, SR, "MOV"},
        [0x8D] = {op_lea, RW, RMW, "LEA"},
        [0x8E] = {op_mov_w, SR, RMW, "MOV"},
        [0x8F] = {op_pop, RMW, Implied, "POP"},

        [0x90] = {op_nop, Implied, Implied, "NOP"},
        [0x93] = {op_xchg_w, R_AX, R_BX, "XCHG"},
        [0x98] = {op_cbw, Implied, Implied, "CBW"},
        [0x99] = {op_cwd, Implied, Implied, "CWD"},
        [0x9C] = {op_pushf, Implied, Implied, "PUSHF"},
        [0x9A] = {op_call_far, Implied, IDW, "CALL"},
        [0x9D] = {op_popf, Implied, Implied, "POPF"},
        [0x9E] = {op_sahf, Implied, Implied, "SAHF"},
        [0x9F] = {op_lahf, Implied, Implied, "LAHF"},

        [0xA1] = {op_mov_w, R_AX, IA, "MOV"},
        [0xA2] = {op_mov_b, IA, R_AL, "MOV"},
        [0xA3] = {op_mov_w, IA, R_AX, "MOV"},
        [0xA4] = {op_movs_b, Implied, Implied, "MOVSB"},
        [0xA5] = {op_movs_w, Implied, Implied, "MOVSW"},
        [0xA6] = {op_cmps_b, Implied, Implied, "CMPSB"},
        [0xA7] = {op_cmps_w, Implied, Implied, "CMPSW"},
        [0xA8] = {op_test_b, R_AL, IB, "TEST"},
        [0xA9] = {op_test_w, R_AX, IW, "TEST"},
        [0xAA] = {op_stos_b, Implied, Implied, "STOSB"},
        [0xAB] = {op_stos_w, Implied, Implied, "STOSW"},
        [0xAC] = {op_lods_b, Implied, Implied, "LODSB"},
        [0xAD] = {op_lods_w, Implied, Implied, "LODSW"},
        [0xAE] = {op_scas_b, Implied, Implied, "SCASB"},
        [0xAF] = {op_scas_w, Implied, Implied, "SCASW"},

        [0xB0] = {op_mov_b, R_AL, IB, "MOV"},
        [0xB1] = {op_mov_b, R_CL, IB, "MOV"},
        [0xB2] = {op_mov_b, R_DL, IB, "MOV"},
        [0xB3] = {op_mov_b, R_BL, IB, "MOV"},
        [0xB4] = {op_mov_b, R_AH, IB, "MOV"},
        [0xB5] = {op_mov_b, R_CH, IB, "MOV"},
        [0xB6] = {op_mov_b, R_DH, IB, "MOV"},
        [0xB8] = {op_mov_w, R_AX, IW, "MOV"},
        [0xB9] = {op_mov_w, R_CX, IW, "MOV"},
        [0xBA] = {op_mov_w, R_DX, IW, "MOV"},
        [0xBB] = {op_mov_w, R_BX, IW, "MOV"},
        [0xBC] = {op_mov_w, R_SP, IW, "MOV"},
        [0xBD] = {op_mov_w, R_BP, IW, "MOV"},
        [0xBE] = {op_mov_w, R_SI, IW, "MOV"},
        [0xBF] = {op_mov_w, R_DI, IW, "MOV"},

        [0xC2] = {op_retn, Implied, IW, "RET"},
        [0xC3] = {op_ret, Implied, Implied, "RET"},
        [0xC4] = {op_les, RW, RMW, "LES"},
        [0xC5] = {op_lds, RW, RMW, "LDS"},
        [0xC6] = {op_mov_b, RMB, IB, "MOV"},
        [0xC7] = {op_mov_w, RMW, IW, "MOV"},
        [0xCA] = {op_retn_far, Implied, IW, "RETF"},
        [0xCB] = {op_ret_far, Implied, Implied, "RETF"},
        [0xCD] = {op_int, Implied, IB, "INT"},
        [0xCE] = {op_into, Implied, Implied, "INTO"},
        [0xCF] = {op_iret, Implied, Implied, "IRET"},

        [0xD4] = {op_aam, Implied, IB, "AAM"},
        [0xD5] = {op_aad, Implied, IB, "AAD"},
        [0xD7] = {op_xlat, Implied, Implied, "XLAT"},

        [0xE0] = {op_loopne, Implied, IB, "LOOPNE"},
        [0xE1] = {op_loope, Implied, IB, "LOOPE"},
        [0xE2] = {op_loop, Implied, IB, "LOOP"},
        [0xE3] = {op_jmp_cxz, Implied, IB, "JCXZ"},
        [0xE8] = {op_call_relative, Implied, IW, "CALL"},
        [0xE9] = {op_jmp_near_relative, Implied, IW, "JMP"},
        [0xEA] = {op_jmp_far, Implied, IDW, "JMP"},
        [0xEB] = {op_jmp_short, Implied, IB, "JMP"},
        [0xEF] = {op_out_w, R_DX, R_AX, "OUT"},

        [0xF2] = {op_repnz, Implied, Implied, "REPNZ"},
        [0xF3] = {op_repz, Implied, Implied, "REPZ"},
        [0xF4] = {op_hlt, Implied, Implied, "HLT"},
        [0xF5] = {op_cmc, Implied, Implied, "CMC"},
        [0xF8] = {op_clc, Implied, Implied, "CLC"},
        [0xF9] = {op_stc, Implied, Implied, "STC"},
        [0xFA] = {op_cli, Implied, Implied, "CLI"},
        [0xFB] = {op_sti, Implied, Implied, "STI"},
        [0xFC] = {op_cld, Implied, Implied, "CLD"},
        [0xFD] = {op_std, Implied, Implied, "STD"},
};

const Opcode opcodes_grp1[] = {
        [000] = {op_add_b, RMB, IB, "ADD"},
        [001] = {op_or_b, RMB, IB, "OR"},
        [002] = {op_adc_b, RMB, IB, "ADC"},
        [003] = {op_sbb_b, RMB, IB, "SBB"},
        [004] = {op_and_b, RMB, IB, "AND"},
        [005] = {op_sub_b, RMB, IB, "SUB"},
        [006] = {op_xor_b, RMB, IB, "XOR"},
        [007] = {op_cmp_b, RMB, IB, "CMP"},

        [010] = {op_add_w, RMW, IW, "ADD"},
        [011] = {op_or_w, RMW, IW, "OR"},
        [012] = {op_adc_w, RMW, IW, "ADC"},
        [013] = {op_sbb_w, RMW, IW, "SBB"},
        [014] = {op_and_w, RMW, IW, "AND"},
        [015] = {op_sub_w, RMW, IW, "SUB"},
        [016] = {op_xor_w, RMW, IW, "XOR"},
        [017] = {op_cmp_w, RMW, IW, "CMP"},

        [030] = {op_add_w, RMW, IB_SE, "ADD"},
        [032] = {op_adc_w, RMW, IB_SE, "ADC"},
        [033] = {op_sub_w, RMW, IB_SE, "SBB"},
        [035] = {op_sub_w, RMW, IB_SE, "SUB"},
        [037] = {op_cmp_w, RMW, IB_SE, "CMP"},
};

const Opcode opcodes_grp2[] = { // octal ;D
        [000] = {op_rol_b, RMB, ONE, "ROL"},
        [001] = {op_ror_b, RMB, ONE, "ROR"},
        [002] = {op_rcl_b, RMB, ONE, "RCL"},
        [003] = {op_rcr_b, RMB, ONE, "RCR"},
        [004] = {op_shl_b, RMB, ONE, "SHL"},
        [005] = {op_shr_b, RMB, ONE, "SHR"},
        [007] = {op_sar_b, RMB, ONE, "SAR"},

        [010] = {op_rol_w, RMW, ONE, "ROL"},
        [011] = {op_ror_w, RMW, ONE, "ROR"},
        [012] = {op_rcl_w, RMW, ONE, "RCL"},
        [013] = {op_rcr_w, RMW, ONE, "RCR"},
        [014] = {op_shl_w, RMW, ONE, "SHL"},
        [015] = {op_shr_w, RMW, ONE, "SHR"},
        [017] = {op_sar_w, RMW, ONE, "SAR"},

        [020] = {op_rol_b, RMB, R_CL, "ROL"},
        [021] = {op_ror_b, RMB, R_CL, "ROR"},
        [022] = {op_rcl_b, RMB, R_CL, "RCL"},
        [023] = {op_rcr_b, RMB, R_CL, "RCR"},
        [024] = {op_shl_b, RMB, R_CL, "SHL"},
        [025] = {op_shr_b, RMB, R_CL, "SHR"},
        [027] = {op_sar_b, RMB, R_CL, "SAR"},

        [030] = {op_rol_w, RMW, R_CL, "ROL"},
        [031] = {op_ror_w, RMW, R_CL, "ROR"},
        [032] = {op_rcl_w, RMW, R_CL, "RCL"},
        [033] = {op_rcr_w, RMW, R_CL, "RCR"},
        [034] = {op_shl_w, RMW, R_CL, "SHL"},
        [035] = {op_shr_w, RMW, R_CL, "SHR"},
        [037] = {op_sar_w, RMW, R_CL, "SAR"},
};

const Opcode opcodes_grp3a[] = {
        [000] = {op_test_b, RMB, IB, "TEST"},
        [002] = {op_not_b, RMB, Implied, "NOT"},
        [003] = {op_neg_b, RMB, Implied, "NEG"},
        [004] = {op_mul_b, Implied, RMB, "MUL"},
        [005] = {op_imul_b, Implied, RMB, "IMUL"},
        [006] = {op_div_b, Implied, RMB, "DIV"},
        [007] = {op_idiv_b, Implied, RMB, "IDIV"},
};

const Opcode opcodes_grp3b[] = {
        [000] = {op_test_w, RMW, IW, "TEST"},
        [002] = {op_not_w, RMW, Implied, "NOT"},
        [003] = {op_neg_w, RMW, Implied, "NEG"},
        [004] = {op_mul_w, Implied, RMW, "MUL"},
        [005] = {op_imul_w, Implied, RMW, "IMUL"},
        [006] = {op_div_w, Implied, RMW, "DIV"},
        [007] = {op_idiv_w, Implied, RMW, "IDIV"},
};

const Opcode opcodes_grp4[] = {
        [000] = {op_inc_b, RMB, Implied, "INC"},
        [001] = {op_dec_b, RMB, Implied, "DEC"},
};

const Opcode opcodes_grp5[] = {
        [000] = {op_inc_w, RMW, Implied, "INC"},
        [001] = {op_dec_w, RMW, Implied, "DEC"},
        [002] = {op_call_direct, Implied, RMW, "CALL"},
        [003] = {op_call_far, Implied, RMW, "CALL"},
        [004] = {op_jmp_near_indirect, Implied, RMW, "JMP"},
        [005] = {op_jmp_far, Implied, RMW, "JMP"},
        [006] = {op_push, Implied, RMW, "PUSH"},
};

bool has_mod_rm_byte(Opcode opcode) {
    return opcode.write_op == RMW || opcode.read_op == RMW ||
           opcode.write_op == RW || opcode.read_op == RW ||
           opcode.write_op == RB || opcode.read_op == RB ||
           opcode.write_op == RMB || opcode.read_op == RMB;
}

Opcode fetch_opcode(Machine *machine, u8 opcode_num) {
    AddrMode addr_mode = peek_addressing_mode(machine);
    switch (opcode_num) {
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83: {
            u8 grp_idx = ((opcode_num - 0x80) * 010) + addr_mode.reg_sreg; // octal
            // opcode_grp1 maps 80 -> 00..07
            // opcode_grp1 maps 81 -> 010..017
            // opcode_grp1 maps 82 -> 020..027
            // opcode_grp1 maps 83 -> 030..037
            Opcode grp_opc = opcodes_grp1[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(machine, __FILE__, __LINE__);
                cpu_note(machine, "group 1 opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }
        case 0xD0:
        case 0xD1:
        case 0xD2:
        case 0xD3: {
            u8 grp_idx = ((opcode_num - 0xD0) * 010) + addr_mode.reg_sreg; // octal
            Opcode grp_opc = opcodes_grp2[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(machine, __FILE__, __LINE__);
                cpu_note(machine, "group 2 opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }
        case 0xF6: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp3a[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(machine, __FILE__, __LINE__);
                cpu_note(machine, "group 3a opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }
        case 0xF7: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp3b[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(machine, __FILE__, __LINE__);
                cpu_note(machine, "group 3b opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;

        }
        case 0xFE: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp4[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(machine, __FILE__, __LINE__);
                cpu_note(machine, "group 4 opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }
        case 0xFF: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp5[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(machine, __FILE__, __LINE__);
                cpu_note(machine, "group 5 opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }

        default:
            return opcodes[opcode_num];
    }
}

void decode_read_op(Machine *machine, AddressOperandCode read_op) {
    CPU *cpu = machine->cpu;
    Memory *mem = machine->memory;

    Operand *op = &cpu->read_op;
    op->dword_cache = 0;
    op->displacement = 0;
    op->word = 0;

    switch (read_op) {
        case Implied: {
            break;
        }
        case ONE: {
            op->word_cache = 1;
            op->word = (u16 *) &op->word_cache;
            break;
        }
        case IA: {
            u16 offset = read_memory_u16(cpu_ip(cpu), mem);
            u32 addr = effective_addr(machine, offset, DS_SEGMENT);

            op->word_cache = offset;
            op->word = (u16 *) &mem->ram[addr];

            cpu->IP += 2;
            break;
        }
        case IB: {
            u32 addr = cpu_ip(cpu);
            u8 b = read_memory_u8(addr, mem);
            op->byte_cache = b;
            op->byte = (u8 *) &op->byte_cache;
            cpu->IP += 1;
            break;
        }
        case IB_SE: {
            u32 addr = cpu_ip(cpu);
            u8 lo_byte = read_memory_u8(addr, mem);
            u8 hi_byte = ((lo_byte & 0x80) == 0x80) ? 0xFF : 0x00;
            u16 word = (hi_byte << 8) | lo_byte;
            op->word_cache = word;
            op->word = (u16 *) &op->word_cache;
            cpu->IP += 1;
            break;
        }
        case IW: {
            u32 addr = cpu_ip(cpu);
            u16 word = read_memory_u16(addr, mem);
            op->word_cache = word;
            op->word = (u16 *) &op->word_cache;
            cpu->IP += 2;
            break;
        }
        case IDW: {
            // fixme: less hacky solution?
            u32 addr = cpu_ip(cpu);
            u16 w1 = read_memory_u16(addr, mem);
            u16 w2 = read_memory_u16(addr + 2, mem);
            cpu->IP += 4;
            op->dword_cache = (w2 << 16) | w1;  // pack value for disassembly
            op->word = (u16 *) &mem->ram[addr];
            break;
        }
        case R_AX: {
            op->word = &cpu->AX;
            break;
        }
        case R_AL: {
            op->byte = &cpu->AL;
            break;
        }
        case R_BX: {
            op->word = &cpu->BX;
            break;
        }
        case R_CX: {
            op->word = &cpu->CX;
            break;
        }
        case R_CL: {
            op->byte = &cpu->CL;
            break;
        }
        case R_DX: {
            op->word = &cpu->DX;
            break;
        }
        case R_DI: {
            op->word = &cpu->DI;
            break;
        }
        case R_ES: {
            op->word = &cpu->ES;
            break;
        }
        case RB:
        case RW: {
            OperandSize op_size = BYTE;
            if (read_op == RW) {
                op_size = WORD;
            }
            REG reg = (op_size << 3) | cpu->addr_mode.reg_sreg;
            decode_register(machine, reg, op);
            break;
        }
        case RMB:
        case RMW: {
            OperandSize op_size = BYTE;
            if (read_op == RMW) {
                op_size = WORD;
            }

            MemoryMode memory_mode;
            memory_mode.memory_mode = cpu->addr_mode.reg_mem;
            memory_mode.operand_size = op_size;

            u8 mode = cpu->addr_mode.mode;
            if (mode == 0) {
                memory_mode.displacement_type = NO_DISP;
                decode_operand(machine, memory_mode, op);
                break;
            } else if (mode == 1) {
                memory_mode.displacement_type = BYTE_DISP;
                decode_operand(machine, memory_mode, op);
                break;
            } else if (mode == 2) {
                memory_mode.displacement_type = WORD_DISP;
                decode_operand(machine, memory_mode, op);
                break;
            } else if (mode == 3) {
                REG reg = (op_size << 3) | memory_mode.memory_mode;
                decode_register(machine, reg, op);
                break;
            }
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_note(machine, "Unsupported mode %d", mode);
            cpu_error(machine, "Unhandled read RM%c mode", (op_size ? 'W' : 'B'));
        }
        case SR: {
            SegREG reg = cpu->addr_mode.reg_sreg;
            decode_segment_register(machine, reg, op);
            break;
        }
        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "unsupported read operation '%d'!", read_op);
        }
    }
}

void decode_write_op(Machine *machine, AddressOperandCode write_op) {
    CPU *cpu = machine->cpu;
    Memory *mem = machine->memory;

    Operand *op = &cpu->write_op;
    op->dword_cache = 0;
    op->displacement = 0;
    op->word = 0;

    switch (write_op) {
        case Implied: {
            break;
        }
        case IA: {
            // The instruction has an immediate word that is an address to a value
            u16 offset = read_memory_u16(cpu_ip(cpu), mem);
            u32 addr = effective_addr(machine, offset, DS_SEGMENT);
            op->word_cache = offset;
            op->word = (u16 *) &mem->ram[addr];
            cpu->IP += 2;
            break;
        }
        case R_AX: {
            op->word = &cpu->AX;
            break;
        }
        case R_AL: {
            op->byte = &cpu->AL;
            break;
        }
        case R_AH: {
            op->byte = &cpu->AH;
            break;
        }
        case R_BX: {
            op->word = &cpu->BX;
            break;
        }
        case R_BL: {
            op->byte = &cpu->BL;
            break;
        }
        case R_CX: {
            op->word = &cpu->CX;
            break;
        }
        case R_CL: {
            op->byte = &cpu->CL;
            break;
        }
        case R_CH: {
            op->byte = &cpu->CH;
            break;
        }
        case R_DX: {
            op->word = &cpu->DX;
            break;
        }
        case R_DH: {
            op->byte = &cpu->DH;
            break;
        }
        case R_DL: {
            op->byte = &cpu->DL;
            break;
        }
        case R_BP: {
            op->word = &cpu->BP;
            break;
        }
        case R_SP: {
            op->word = &cpu->SP;
            break;
        }
        case R_SI: {
            op->word = &cpu->SI;
            break;
        }
        case R_DI: {
            op->word = &cpu->DI;
            break;
        }
        case R_ES: {
            op->word = &cpu->ES;
            break;
        }
        case RMB:
        case RMW: {
            OperandSize op_size = BYTE;
            if (write_op == RMW) {
                op_size = WORD;
            }

            MemoryMode memory_mode;
            memory_mode.memory_mode = cpu->addr_mode.reg_mem;
            memory_mode.operand_size = op_size;

            u8 mode = cpu->addr_mode.mode;
            if (mode == 0) {
                memory_mode.displacement_type = NO_DISP;
                decode_operand(machine, memory_mode, op);
                break;
            } else if (mode == 1) {
                memory_mode.displacement_type = BYTE_DISP;
                decode_operand(machine, memory_mode, op);
                break;
            } else if (mode == 3) {
                REG reg = (op_size << 3) | memory_mode.memory_mode;
                decode_register(machine, reg, op);
                break;
            }
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_note(machine, "Unsupported mode %d", mode);
            cpu_note(machine, "Current opcode 0x%02X", cpu->opcode);
            cpu_peek(machine);
            cpu_error(machine, "Unhandled write RM%c mode", (op_size ? 'W' : 'B'));

        }
        case RB:
        case RW: {
            OperandSize op_size = BYTE;
            if (write_op == RW) {
                op_size = WORD;
            }
            REG reg = (op_size << 3) | cpu->addr_mode.reg_sreg;
            decode_register(machine, reg, op);
            break;
        }
        case SR: {
            SegREG reg = cpu->addr_mode.reg_sreg;
            decode_segment_register(machine, reg, op);
            break;
        }
        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "unsupported write operation '%d'!", write_op);
        }
    }
}

u8 peek_opcode(Machine *machine) {
    return machine->memory->ram[cpu_ip(machine->cpu)];
}
