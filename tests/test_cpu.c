#include <stdlib.h>
#include "testing.h"
#include "cpu.h"

void test_registers(Tester *tester) {
    test_section("CPU Registers");
    CPU cpu = {0};

    cpu.AX = 0xABBA;
    test_u64(tester, cpu.AH, 0xAB, "AH");
    test_u64(tester, cpu.AL, 0xBA, "AL");
    test_u64(tester, cpu.AX, 0xABBA, "AX");

    cpu.BX = 0xDEAD;
    test_u64(tester, cpu.BH, 0xDE, "BH");
    test_u64(tester, cpu.BL, 0xAD, "BL");
    test_u64(tester, cpu.BX, 0xDEAD, "BX");

    cpu.CX = 0xBEED;
    test_u64(tester, cpu.CH, 0xBE, "CH");
    test_u64(tester, cpu.CL, 0xED, "CL");
    test_u64(tester, cpu.CX, 0xBEED, "CX");

    cpu.DX = 0xFAFA;
    test_u64(tester, cpu.DH, 0xFA, "DH");
    test_u64(tester, cpu.DL, 0xFA, "DL");
    test_u64(tester, cpu.DX, 0xFAFA, "DX");

    cpu.flags.word = 0xFFFF;
    test_u64(tester, cpu.flags.CF, 0x1, "CF");
    test_u64(tester, cpu.flags.PF, 0x1, "PF");
    test_u64(tester, cpu.flags.AF, 0x1, "AF");
    test_u64(tester, cpu.flags.ZF, 0x1, "ZF");
    test_u64(tester, cpu.flags.SF, 0x1, "SF");
    test_u64(tester, cpu.flags.TF, 0x1, "TF");
    test_u64(tester, cpu.flags.IF, 0x1, "IF");
    test_u64(tester, cpu.flags.DF, 0x1, "DF");
    test_u64(tester, cpu.flags.OF, 0x1, "OF");

    cpu.flags.CF = 0x0; // bit 0
    cpu.flags.PF = 0x0; // bit 2
    cpu.flags.AF = 0x0; // bit 4
    cpu.flags.ZF = 0x0; // bit 6
    cpu.flags.SF = 0x0; // bit 7
    cpu.flags.TF = 0x0; // bit 8
    cpu.flags.IF = 0x0; // bit 9
    cpu.flags.DF = 0x0; // bit 10
    cpu.flags.OF = 0x0; // bit 11

    test_u64(tester, cpu.flags.word, 0xFFFF - 0x0FD5, "Flags");

    test_u64(tester, cpu.SI, 0x0, "SI");
    test_u64(tester, cpu.DI, 0x0, "DI");
    test_u64(tester, cpu.BP, 0x0, "BP");
    test_u64(tester, cpu.SP, 0x0, "SP");
    test_u64(tester, cpu.IP, 0x0, "IP");
    test_u64(tester, cpu.CS, 0x0, "CS");
    test_u64(tester, cpu.DS, 0x0, "DS");
    test_u64(tester, cpu.ES, 0x0, "ES");
    test_u64(tester, cpu.SS, 0x0, "SS");
}

void test_enums(Tester *tester) {
    test_section("Opcode Enums");

    // Main and index registers
    test_u64(tester, AL, 0, "AL");
    test_u64(tester, CL, 1, "CL");
    test_u64(tester, DL, 2, "DL");
    test_u64(tester, BL, 3, "BL");

    test_u64(tester, AH, 4, "AH");
    test_u64(tester, CH, 5, "CH");
    test_u64(tester, DH, 6, "DH");
    test_u64(tester, BH, 7, "BH");

    test_u64(tester, AX, 8, "AX");
    test_u64(tester, CX, 9, "CX");
    test_u64(tester, DX, 10, "DX");
    test_u64(tester, BX, 11, "BX");

    test_u64(tester, SP, 12, "SP");
    test_u64(tester, BP, 13, "BP");
    test_u64(tester, SI, 14, "SI");
    test_u64(tester, DI, 15, "DI");

    // Segment registers
    test_u64(tester, ES, 0, "ES");
    test_u64(tester, CS, 1, "CS");
    test_u64(tester, SS, 2, "SS");
    test_u64(tester, DS, 3, "DS");
}

void test_result(Tester *tester) {
    test_section("Operand Union");

    ReadOperand op;
    op.word = 0xDECA;

    test_u64(tester, op.word, 0xDECA, "0xDECA");
    test_u64(tester, op.byte, 0xCA, "0xCA");

    op.byte = 0xAD;
    test_u64(tester, op.word, 0xDEAD, "0xDEAD");
    test_u64(tester, op.byte, 0xAD, "0xAD");

    op.word = 0x00FF;
    op.byte++;
    test_u64(tester, op.word, 0x0000, "0x0000");
    test_u64(tester, op.byte, 0x00, "0x00");

    op.word = 0x00FF;
    op.word++;
    test_u64(tester, op.word, 0x0100, "0x0100");
    test_u64(tester, op.byte, 0x00, "0x00");

}

void test_opcode_decoding(Tester *tester) {
    test_section("Opcode Decoding");
    CPU cpu = {0};
    Memory memory = {0};
    memory.ram[0] = 0xBC;
    memory.ram[1] = 0xA0;
    memory.ram[2] = 0x00;
    memory.ram[3] = 0xB8;
    memory.ram[4] = 0xFF;
    memory.ram[5] = 0xFF;
    memory.ram[6] = 0xBB;
    memory.ram[7] = 0x01;
    memory.ram[8] = 0x00;
    memory.ram[9] = 0x01;
    memory.ram[10] = 0xc3;
    memory.ram[11] = 0xa3;
    memory.ram[12] = 0x00;
    memory.ram[13] = 0x00;

    test_u64(tester, cpu.IP, 0, "IP=0");
    cpu_tick(&cpu, &memory);
    test_u64(tester, cpu.IP, 3, "IP=3");
    test_u64(tester, cpu.SP, 0x00A0, "SP=0x00A0 [160]");

    cpu_tick(&cpu, &memory);
    test_u64(tester, cpu.IP, 6, "IP=6");
    test_u64(tester, cpu.AX, 0xFFFF, "AX=0xFFFF [65535]");

    cpu_tick(&cpu, &memory);
    test_u64(tester, cpu.IP, 9, "IP=9");
    test_u64(tester, cpu.BX, 0x0001, "BX=0x0001 [1]");

    cpu_tick(&cpu, &memory);
    test_u64(tester, cpu.IP, 11, "IP=11");
    test_u64(tester, cpu.BX, 0x0000, "BX=0x0000 [0]");
    test(tester, cpu.flags.CF, "Carry Flag Set");
    test(tester, cpu.flags.PF, "Parity Flag Set");
    test(tester, cpu.flags.AF, "Auxiliary Flag Set");
    test(tester, !cpu.flags.SF, "Sign Flag Not Set");
    test(tester, cpu.flags.ZF, "Zero Flag Set");
    test(tester, !cpu.flags.OF, "Overflow Flag Not Set");

    cpu_tick(&cpu, &memory);
    test_u64(tester, cpu.IP, 14, "IP=14");
    test_u64(tester, memory.ram[0], 0xFF, "[0000]=0xFF [255]");
    test_u64(tester, memory.ram[1], 0xFF, "[0001]=0xFF [255]");

}

void test_operand_order(Tester *tester) {
    test_section("Operand Order");
    CPU cpu = {0};
    Memory memory = {0};
    // MOV word[0x0012], 0x8960
    // C7 06 12 00 60 89
    memory.ram[0] = 0xC7;
    memory.ram[1] = 0x06;
    memory.ram[2] = 0x12;
    memory.ram[3] = 0x00;
    memory.ram[4] = 0x60;
    memory.ram[5] = 0x89;

    cpu_tick(&cpu, &memory);

    test_u64(tester, *((u16 *) &memory.ram[0x0012]), 0x8960, "Ram $0012");
    test_u64(tester, *((u16 *) &memory.ram[0x8960]), 0x0000, "Ram $8960");
}

void test_segments(Tester *tester) {
    test_section("Segments");

    CPU cpu;
    cpu_reset(&cpu);

    test_u64(tester, cpu_ip(&cpu), 0xFFFF0, "Start vector");
    cpu.IP++;
    test_u64(tester, cpu_ip(&cpu), 0xFFFF1, "Next IP");
    test_u64(tester, cpu_ds(&cpu, 0xFF), 0x000FF, "Data segment");
    cpu.DS = 0xA000;
    test_u64(tester, cpu_ds(&cpu, 0xFF), 0xA00FF, "Data segment at 0xA000");
}


int main() {
    Tester tester = create_tester("CPU Test");

    test_registers(&tester);
    test_enums(&tester);
    test_segments(&tester);
    test_result(&tester);
    test_opcode_decoding(&tester);
    test_operand_order(&tester);

    test_summary(&tester);

    exit(tester.fail);
}
