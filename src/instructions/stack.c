#include "instructions.h"

/// Returns a pointer to the top of the stack
static u16 *stack_pointer(Machine *machine) {
    u32 sp = cpu_sp(machine->cpu);
    return (u16 *) &machine->memory->ram[sp];
}

u16 peek_stack(Machine *machine) {
    return read_memory_u16(cpu_sp(machine->cpu), machine->memory);
}

void op_pushf(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    (void) wop;
    m->cpu->SP -= 2;
    u16 *ptr = stack_pointer(m);
    *(ptr) = m->cpu->flags.word;
}

void op_popf(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    (void) rop;
    m->cpu->flags.word = (peek_stack(m) & 0x0FD5) | 0x2; // clear unused bits and set 2 bit to 1
    m->cpu->SP += 2;
}

void op_push(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) wop;
    m->cpu->SP -= 2;
    u16 *ptr = stack_pointer(m);
    *(ptr) = *rop.word;
    //cpu_note_u32(m, "PUSH 0x%04X", *rop.word);
}

void op_pop(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    //cpu_note_u32(m, "POP 0x%04X", peek_stack(m));
    *wop.word = peek_stack(m);
    m->cpu->SP += 2;
}
