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
    //dump_cpu(m);
}

void op_pop(Machine *m, ReadOperand rop, WriteOperand wop) {
    (void) rop;
    *wop.word = peek_stack(m);
    m->cpu->SP += 2;
}

