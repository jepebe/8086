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

        [0x50] = {op_push, Implied, R_AX, "PUSH"},
        [0x51] = {op_push, Implied, R_CX, "PUSH"},
        [0x52] = {op_push, Implied, R_DX, "PUSH"},
        [0x57] = {op_push, Implied, R_DI, "PUSH"},
        [0x58] = {op_pop, R_AX, Implied, "POP"},
        [0x59] = {op_pop, R_CX, Implied, "POP"},
        [0x5B] = {op_pop, R_BX, Implied, "POP"},
        [0x5F] = {op_pop, R_DI, Implied, "POP"},

        [0x74] = {op_jz, Implied, IB, "JZ"},

        [0x84] = {op_test_b, RMB, RB, "TEST"},
        [0x85] = {op_test_w, RMW, RW, "TEST"},
        [0x88] = {op_mov_b, RMB, RB, "MOV"},
        [0x89] = {op_mov_w, RMW, RW, "MOV"},
        [0x8B] = {op_mov_w, RW, RMW, "MOV"},
        [0x8C] = {op_mov_w, RMW, SR, "MOV"},
        [0x8E] = {op_mov_w, SR, RMW, "MOV"},

        [0x90] = {op_nop, Implied, Implied, "NOP"},
        [0x9C] = {op_pushf, Implied, Implied, "PUSHF"},
        [0x9D] = {op_popf, Implied, Implied, "POPF"},
        [0x9F] = {op_lahf, Implied, Implied, "LAHF"},

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

        [0xC3] = {op_ret, Implied, Implied, "RET"},
        [0xC6] = {op_mov_b, RMB, IB, "MOV"},
        [0xC7] = {op_mov_w, RMW, IW, "MOV"},
        [0xCD] = {op_int, Implied, IB, "INT"},
        [0xCE] = {op_into, Implied, Implied, "INTO"},
        [0xCF] = {op_iret, Implied, Implied, "IRET"},

        [0xD4] = {op_aam, Implied, IB, "AAM"},
        [0xD5] = {op_aad, Implied, IB, "AAD"},

        [0xEB] = {op_jmp_short, Implied, IB, "JMP"},

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
        [004] = {op_jmp_near, Implied, RMW, "JMP"},
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
                cpu_note_int(machine, "group 1 opcode 0%02o not implemented", grp_idx);
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
                cpu_note_int(machine, "group 2 opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }
        case 0xF6: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp3a[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(machine, __FILE__, __LINE__);
                cpu_note_int(machine, "group 3a opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }
        case 0xF7: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp3b[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(machine, __FILE__, __LINE__);
                cpu_note_int(machine, "group 3b opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;

        }
        case 0xFE: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp4[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(machine, __FILE__, __LINE__);
                cpu_note_int(machine, "group 4 opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }
        case 0xFF: {
            u8 grp_idx = addr_mode.reg_sreg;
            Opcode grp_opc = opcodes_grp5[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(machine, __FILE__, __LINE__);
                cpu_note_int(machine, "group 5 opcode 0%02o not implemented", grp_idx);
            }
            return grp_opc;
        }

        default:
            return opcodes[opcode_num];
    }
}

ReadOperand decode_read_op(Machine *machine, AddressOperandCode read_op) {
    CPU *cpu = machine->cpu;
    Memory *mem = machine->memory;

    switch (read_op) {
        case Implied: {
            return (ReadOperand) {0};
        }
        case ONE: {
            return (ReadOperand) {.word = 1};
        }
        case IB: {
            u8 b = read_memory_u8(cpu_ip(cpu), mem);
            cpu->immediate_read = b;
            cpu->IP += 1;
            return (ReadOperand) {.byte = b};
        }
        case IB_SE: {
            u8 lo_byte = read_memory_u8(cpu_ip(cpu), mem);
            u8 hi_byte = ((lo_byte & 0x80) == 0x80) ? 0xFF : 0x00;

            u16 word = (hi_byte << 8) | lo_byte;
            cpu->immediate_read = word;
            cpu->IP += 1;
            return (ReadOperand) {.word = word};
        }
        case IW: {
            u16 w = read_memory_u16(cpu_ip(cpu), mem);
            cpu->immediate_read = w;
            cpu->IP += 2;
            return (ReadOperand) {.word = w};
        }
        case R_AX: {
            return (ReadOperand) {.word = cpu->AX};
        }
        case R_AL: {
            return (ReadOperand) {.word = cpu->AL};
        }
        case R_CX: {
            return (ReadOperand) {.word = cpu->CX};
        }
        case R_CL: {
            return (ReadOperand) {.word = cpu->CL};
        }
        case R_DX: {
            return (ReadOperand) {.word = cpu->DX};
        }
        case R_DI: {
            return (ReadOperand) {.word = cpu->DI};
        }
        case RB:
        case RW: {
            OperandSize op_size = BYTE;
            if (read_op == RW) {
                op_size = WORD;
            }
            REG reg = (op_size << 3) | cpu->addr_mode.reg_sreg;
            return get_read_register(machine, reg);
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
                return get_read_memory(machine, memory_mode);
            } else if (mode == 1) {
                memory_mode.displacement_type = BYTE_DISP;
                return get_read_memory(machine, memory_mode);
            } else if (mode == 2) {
                memory_mode.displacement_type = WORD_DISP;
                return get_read_memory(machine, memory_mode);
            } else if (mode == 3) {
                REG reg = (op_size << 3) | memory_mode.memory_mode;
                return get_read_register(machine, reg);
            }
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_note_int(machine, "Unsupported mode %d", mode);
            cpu_error(machine, "Unhandled read RM%c mode", (op_size ? 'W' : 'B'));
        }
        case SR: {
            SegREG reg = cpu->addr_mode.reg_sreg;
            return get_read_segment_register(machine, reg);
        }
        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error_int(machine, "unsupported read operation '%d'!", read_op);
        }
    }
}

WriteOperand decode_write_op(Machine *machine, AddressOperandCode write_op) {
    CPU *cpu = machine->cpu;
    Memory *mem = machine->memory;

    switch (write_op) {
        case Implied: {
            return (WriteOperand) {0};
        }
        case IA: {
            u16 offset = read_memory_u16(cpu_ip(cpu), mem);
            cpu->immediate_write = offset;
            cpu->IP += 2;
            u32 addr = cpu_ds(cpu, offset);
            return (WriteOperand) {.word = (uint16_t *) &mem->ram[addr]};
        }
        case R_AX: {
            return (WriteOperand) {.word =  &cpu->AX};
        }
        case R_AL: {
            return (WriteOperand) {.byte =  &cpu->AL};
        }
        case R_AH: {
            return (WriteOperand) {.byte =  &cpu->AH};
        }
        case R_BX: {
            return (WriteOperand) {.word =  &cpu->BX};
        }
        case R_BL: {
            return (WriteOperand) {.byte =  &cpu->BL};
        }
        case R_CX: {
            return (WriteOperand) {.word =  &cpu->CX};
        }
        case R_CL: {
            return (WriteOperand) {.byte =  &cpu->CL};
        }
        case R_CH: {
            return (WriteOperand) {.byte =  &cpu->CH};
        }
        case R_DX: {
            return (WriteOperand) {.word =  &cpu->DX};
        }
        case R_DH: {
            return (WriteOperand) {.byte =  &cpu->DH};
        }
        case R_DL: {
            return (WriteOperand) {.byte =  &cpu->DL};
        }
        case R_BP: {
            return (WriteOperand) {.word =  &cpu->BP};
        }
        case R_SP: {
            return (WriteOperand) {.word =  &cpu->SP};
        }
        case R_SI: {
            return (WriteOperand) {.word =  &cpu->SI};
        }
        case R_DI: {
            return (WriteOperand) {.word =  &cpu->DI};
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
                return get_write_memory(machine, memory_mode);
            } else if (mode == 1) {
                memory_mode.displacement_type = BYTE_DISP;
                return get_write_memory(machine, memory_mode);
            } else if (mode == 3) {
                REG reg = (op_size << 3) | memory_mode.memory_mode;
                return get_write_register(machine, reg);
            }
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_note_int(machine, "Unsupported mode %d", mode);
            cpu_note_int(machine, "Current opcode 0x%02X", cpu->opcode);
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
            // printf("reg = %s [%d]\n", get_register_name(reg), reg);
            return get_write_register(machine, reg);
        }
        case SR: {
            SegREG reg = cpu->addr_mode.reg_sreg;
            return get_write_segment_register(machine, reg);
        }
        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error_int(machine, "unsupported write operation '%d'!", write_op);
        }
    }
}

static const char *AOC_TABLE[] = {
        [Implied] = "",
        [ONE] = "1",
        [IA] = "IA=0x%04X",
        [IB] = "IB=0x%02X",
        [IB_SE] = "IBSE=0x%02X",
        [IW] = "IW=0x%04X",
        [R_AX] = "AX",
        [R_AL] = "AL",
        [R_AH] = "AH",
        [R_BX] = "BX",
        [R_BL] = "BL",
        [R_CX] = "CX",
        [R_CL] = "CL",
        [R_CH] = "CH",
        [R_DX] = "DX",
        [R_DL] = "DL",
        [R_DH] = "DH",
        [R_BP] = "BP",
        [R_SP] = "SP",
        [R_SI] = "SI",
        [R_DI] = "DI",
};

const char *decode_aoc(Machine *machine, AddressOperandCode aoc) {
    CPU *cpu = machine->cpu;

    const char *repr = AOC_TABLE[aoc];
    if (repr) {
        return repr;
    }
    switch (aoc) {
        case RB:
        case RW: {
            bool word = false;
            if (aoc == RW) {
                word = true;
            }
            REG reg = (word << 3) | cpu->addr_mode.reg_sreg;
            return get_register_name(reg);
        }
        case RMB:
        case RMW: {
            bool word = false;
            if (aoc == RMW) {
                word = true;
            }
            u8 mode = cpu->addr_mode.mode;
            if (mode == 0) {
                MemoryModeTable mem_mode = cpu->addr_mode.reg_mem;
                return get_memory_mode_table_name(mem_mode, NO_DISP);

            } else if (mode == 1) {
                MemoryModeTable mem_mode = cpu->addr_mode.reg_mem;
                return get_memory_mode_table_name(mem_mode, BYTE_DISP);

            } else if (mode == 2) {
                MemoryModeTable mem_mode = cpu->addr_mode.reg_mem;
                return get_memory_mode_table_name(mem_mode, WORD_DISP);

            } else if (mode == 3) {
                REG reg = (word << 3) | cpu->addr_mode.reg_mem;
                return get_register_name(reg);
            }
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_note_int(machine, "Unsupported mode %d", mode);
            cpu_error(machine, "Unhandled RM%c mode", (word ? 'W' : 'B'));
        }
        case SR: {
            SegREG reg = cpu->addr_mode.reg_sreg;
            return get_segment_register_name(reg);
        }

        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unhandled AddressOperandCode mode '%d'", aoc);
        }
    }
}


