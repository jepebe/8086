#include "instructions.h"

void op_in_b_portb(Machine *m, Operand *rop, Operand *wop) {
    // Input byte from I/O port in DX into AL.
    u8 port = *rop->byte;
    *wop->byte = 0x00; //todo read from where?
    cpu_note(m, "IN AL=0x%02X from port=0x%02X", 0x00, port);
}
void op_in_b_portw(Machine *m, Operand *rop, Operand *wop) {
    // Input byte from I/O port in DX into AL.
    u16 port = *rop->word;
    *wop->byte = 0x00; //todo read from where?
    cpu_note(m, "IN AL=0x%02X from port=0x%04X", 0x00, port);
}

void op_out_w_portw(Machine *m, Operand *rop, Operand *wop) {
    // Output word in AX to I/O port address in DX.
    u16 data = *rop->word;
    u16 port = *wop->word;
    cpu_note(m, "OUT AX=0x%04X to port=0x%04X", data, port);
}

void op_out_b_portw(Machine *m, Operand *rop, Operand *wop) {
    // Output byte in AL to I/O port address in DX.
    u8 data = *rop->byte;
    u16 port = *wop->word;
    cpu_note(m, "OUT AL=0x%02X to port=0x%04X", data, port);
}

void op_out_b_portb(Machine *m, Operand *rop, Operand *wop) {
    // Output byte in AL to I/O port address imm8.
    u8 data = *rop->byte;
    u8 port = *wop->byte;
    cpu_note(m, "OUT AL=0x%02X to port=0x%02X", data, port);
}
