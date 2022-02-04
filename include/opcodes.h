#include "cpu.h"
#include "instructions.h"

const Opcode opcodes[256] = {
        [0x00] = {op_add_b, RMB, RB, "ADD"},
        [0x01] = {op_add_w, RMW, RW, "ADD"},
        [0x02] = {op_add_b, RB, RMB, "ADD"},
        [0x03] = {op_add_w, RW, RMW, "ADD"},
        [0x04] = {op_add_b, R_AL, IB, "ADD"},
        [0x05] = {op_add_w, R_AX, IW, "ADD"},
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
        [0x28] = {op_sub_b, RMB, RB, "SUB"},
        [0x29] = {op_sub_w, RMW, RW, "SUB"},
        [0x2A] = {op_sub_b, RB, RMB, "SUB"},
        [0x2B] = {op_sub_w, RW, RMW, "SUB"},
        [0x2C] = {op_sub_b, R_AL, IB, "SUB"},

        [0x30] = {op_xor_b, RMB, RB, "XOR"},
        [0x31] = {op_xor_w, RMW, RW, "XOR"},
        [0x32] = {op_xor_b, RB, RMB, "XOR"},
        [0x33] = {op_xor_w, RW, RMW, "XOR"},
        [0x34] = {op_xor_b, R_AL, IB, "XOR"},
        [0x35] = {op_xor_w, R_AX, IW, "XOR"},
        [0x38] = {op_cmp_b, RMB, RB, "CMP"},
        [0x39] = {op_cmp_w, RMW, RW, "CMP"},
        [0x3A] = {op_cmp_b, RB, RMB, "CMP"},
        [0x3C] = {op_cmp_b, R_AL, IB, "CMP"},

        [0x47] = {op_inc_w, R_DI, Implied, "INC"},
        [0x4F] = {op_dec_w, R_DI, Implied, "DEC"},

        [0x50] = {op_push, SP_PTR, R_AX, "PUSH"},
        [0x51] = {op_push, SP_PTR, R_CX, "PUSH"},
        [0x52] = {op_push, SP_PTR, R_DX, "PUSH"},
        [0x58] = {op_pop, R_AX, SP_PTR, "POP"},
        [0x5B] = {op_pop, R_BX, SP_PTR, "POP"},

        [0x84] = {op_test_b, RMB, RB, "TEST"},
        [0x85] = {op_test_w, RMW, RW, "TEST"},
        [0x88] = {op_mov_b, RMB, RB, "MOV"},
        [0x89] = {op_mov_w, RMW, RW, "MOV"},
        [0x8E] = {op_mov_w, SR, RMW, "MOV"},

        [0x90] = {op_nop, Implied, Implied, "NOP"},
        [0x9C] = {op_pushf, SP_PTR, Implied, "PUSHF"},
        [0x9D] = {op_popf, Implied, SP_PTR, "POPF"},

        [0xA2] = {op_mov_b, IA, R_AL, "MOV"},
        [0xA3] = {op_mov_w, IA, R_AX, "MOV"},
        [0xA8] = {op_test_b, R_AL, IB, "TEST"},
        [0xA9] = {op_test_w, R_AX, IW, "TEST"},

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

        [0xC6] = {op_mov_w, RMB, IB, "MOV"},
        [0xC7] = {op_mov_w, RMW, IW, "MOV"},

        [0xD5] = {op_aad, Implied, IB, "AAD"},

        [0xEB] = {op_jmp, Implied, IB, "JMP"},
        [0xF4] = {op_hlt, Implied, Implied, "HLT"},
        [0xF5] = {op_cmc, Implied, Implied, "CMC"},
        [0xF8] = {op_clc, Implied, Implied, "CLC"},
        [0xF9] = {op_stc, Implied, Implied, "STC"},
        [0xFA] = {op_cli, Implied, Implied, "CLI"},
        [0xFB] = {op_sti, Implied, Implied, "STI"},
        [0xFC] = {op_cld, Implied, Implied, "CLD"},
        [0xFD] = {op_std, Implied, Implied, "STD"},
};

const Opcode opcodes_grp1[040] = {
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

const Opcode opcodes_grp2[040] = { // octal ;D
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

const Opcode opcodes_grp3a[010] = {
        [000] = {op_test_b, RMB, IB, "TEST"},
        [002] = {op_not_b, RMB, Implied, "NOT"},
        [003] = {op_neg_b, RMB, Implied, "NEG"},
        [004] = {op_mul_b, Implied, RMB, "MUL"},
        [005] = {op_imul_b, Implied, RMB, "IMUL"},
};

const Opcode opcodes_grp3b[010] = {
        [000] = {op_test_w, RMW, IW, "TEST"},
        [002] = {op_not_w, RMW, Implied, "NOT"},
        [003] = {op_neg_w, RMW, Implied, "NEG"},
        [004] = {op_mul_w, Implied, RMW, "MUL"},
        [005] = {op_imul_w, Implied, RMW, "IMUL"},
};

const Opcode opcodes_grp4[010] = {
        [000] = {op_inc_b, RMB, Implied, "INC"},
        [001] = {op_dec_b, RMB, Implied, "DEC"},
};

const Opcode opcodes_grp5[010] = {
        [000] = {op_inc_w, RMW, Implied, "INC"},
        [001] = {op_dec_w, RMW, Implied, "DEC"},
};

void fetch_addressing_mode(CPU *cpu, Memory *mem) {
    cpu->addr_mode.opcode = mem->ram[cpu_ip(cpu)];
    cpu->IP++;
}

AddrMode peek_addressing_mode(CPU *cpu, Memory *mem) {
    return (AddrMode) {.opcode = mem->ram[cpu_ip(cpu)]};
}

bool has_mod_rm_byte(Opcode opcode) {
    return opcode.write_op == RMW || opcode.read_op == RMW ||
           opcode.write_op == RW || opcode.read_op == RW ||
           opcode.write_op == RB || opcode.read_op == RB ||
           opcode.write_op == RMB || opcode.read_op == RMB;
}

Opcode fetch_opcode(CPU *cpu, Memory *mem, u8 opcode_num) {
    AddrMode addr_mode = peek_addressing_mode(cpu, mem);
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
                cpu_error_marker(cpu, __FILE__, __LINE__);
                cpu_note_int(cpu, "group 1 opcode 0%02o not implemented", grp_idx);
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
                cpu_error_marker(cpu, __FILE__, __LINE__);
                cpu_note_int(cpu, "group 2 opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }
        case 0xF6: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp3a[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(cpu, __FILE__, __LINE__);
                cpu_note_int(cpu, "group 3a opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }
        case 0xF7: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp3b[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(cpu, __FILE__, __LINE__);
                cpu_note_int(cpu, "group 3b opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;

        }
        case 0xFE: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp4[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(cpu, __FILE__, __LINE__);
                cpu_note_int(cpu, "group 4 opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }
        case 0xFF: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp5[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(cpu, __FILE__, __LINE__);
                cpu_note_int(cpu, "group 5 opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }

        default:
            return opcodes[opcode_num];
    }
}
