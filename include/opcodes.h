#include "cpu.h"
#include "instructions.h"

const Opcode opcodes[] = {
        [0x01] = {op_add_w, RMW, RW, "ADD"},
        [0x03] = {op_add_w, RW, RMW, "ADD"},
        [0x05] = {op_add_w, R_AX, IW, "ADD"},

        [0x89] = {op_mov_w, RMW, RW, "MOV"},

        [0x9C] = {op_pushf, SP_PTR, Implied, "PUSHF"},

        [0xA3] = {op_mov_w, IA, R_AX, "MOV"},

        [0xB6] = {op_mov_b, R_DH, IB, "MOV"},
        [0xBA] = {op_mov_w, R_DX, IW, "MOV"},
        [0xB8] = {op_mov_w, R_AX, IW, "MOV"},
        [0xB9] = {op_mov_w, R_CX, IW, "MOV"},
        [0xBB] = {op_mov_w, R_BX, IW, "MOV"},
        [0xBC] = {op_mov_w, R_SP, IW, "MOV"},
        [0xBD] = {op_mov_w, R_BP, IW, "MOV"},
        [0xBE] = {op_mov_w, R_SI, IW, "MOV"},

        [0xC6] = {op_mov_w, RMB, IB, "MOV"},
        [0xC7] = {op_mov_w, RMW, IW, "MOV"},

        [0xEB] = {op_jmp, Implied, IB, "JMP"},
};

const Opcode opcodes_grp1[] = {
        [0x00] = {op_add_b, RMB, IB, "ADD"},
        [0x10] = {op_add_w, RMW, IW, "ADD"},
        [0x30] = {op_add_w, RMW, IB, "ADD"},
        //todo sign extension missing
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
           opcode.write_op ==RMB || opcode.read_op == RMB;
}

Opcode fetch_opcode(CPU *cpu, Memory *mem, u8 opcode_num) {
    switch (opcode_num) {
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83: {
            AddrMode addr_mode = peek_addressing_mode(cpu, mem);
            u8 grp_idx = ((opcode_num - 0x80) << 4) + addr_mode.reg_sreg;
            // opcode_grp1 maps 80 -> 0..0f
            // opcode_grp1 maps 81 -> 10..1f
            // opcode_grp1 maps 82 -> 20..2f
            // opcode_grp1 maps 83 -> 30..3f
            Opcode grp_opc = opcodes_grp1[grp_idx];
            if (grp_opc.op_fn == 0) {
                cpu_error_marker(cpu, __FILE__, __LINE__);
                cpu_note_int(cpu, "group opcode 0x%02X not implemented", grp_idx);
            }
            return grp_opc;
        }
        default:
            return opcodes[opcode_num];
    }
}
