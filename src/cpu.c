#include "cpu.h"

void cpu_reset(CPU *cpu) {
    cpu->CS = 0xF000;
    cpu->DS = 0x0000;
    cpu->SS = 0x0000;
    cpu->IP = 0xFFF0;
    cpu->halted = false;
    cpu->flags.word = 0x0002; // set first unused bit to 1
    cpu->segment_override = DEFAULT_SEGMENT;
    cpu->read_op = (Operand) {0};
    cpu->write_op = (Operand) {0};
}

u32 cpu_ip(CPU *cpu) {
    return ((cpu->CS << 4) + cpu->IP) & 0xFFFFF;
}

u32 cpu_sp(CPU *cpu) {
    return ((cpu->SS << 4) + cpu->SP) & 0xFFFFF;
}
