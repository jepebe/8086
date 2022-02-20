#include "instructions.h"

void op_out_w(Machine *m, Operand *rop, Operand *wop) {
    // Output word in AX to I/O port address in DX.
    u16 data = *rop->word;
    u16 port = *wop->word;
    cpu_note(m, "OUT AX=0x%04X to DX=0x%04X", data, port);
}
