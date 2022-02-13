#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "machine.h"
#include "opcodes.h"
#include "debug.h"

Machine machine_create() {
    CPU *cpu = malloc(sizeof(CPU));
    cpu_reset(cpu);
    Memory *mem = malloc(sizeof(Memory));
    return (Machine) {.cpu = cpu, .memory = mem};
}

static void disassemble_instruction(Machine *machine, u32 addr, u8 opc, Opcode opcode) {
    printf("[$%05X] ", addr);
    printf("[%02X] ", opc); // Opcode number
    printf("%s ", opcode.name); // opcode mnemonic

    const char *w = decode_aoc(machine, opcode.write_op);

    if (w[0] != '\0') {
        if (strchr(w, '%') != NULL) {
            printf(w, machine->cpu->immediate_write);
        } else {
            printf("%s", w);
        }
    }

    const char *r = decode_aoc(machine, opcode.read_op);

    if (w[0] != '\0' && r[0] != '\0') {
        printf(", ");
    }

    if (r[0] != '\n') {
        if (strchr(r, '%') != NULL) {
            printf(r, machine->cpu->immediate_read);
        } else {
            printf("%s", r);
        }
    }

    printf("\n");
}

void machine_tick(Machine *m) {
    u32 addr = cpu_ip(m->cpu);
    u8 opcode_num = m->memory->ram[addr];
    m->cpu->opcode_ip = m->cpu->IP;
    m->cpu->IP++;
    m->cpu->opcode = opcode_num;
    Opcode opcode = fetch_opcode(m, opcode_num);

    if (opcode.op_fn != NULL) {
        if (has_mod_rm_byte(opcode)) {
            fetch_addressing_mode(m);
        }
        WriteOperand out = decode_write_op(m, opcode.write_op);
        ReadOperand in = decode_read_op(m, opcode.read_op);

        disassemble_instruction(m, addr, opcode_num, opcode);

        opcode.op_fn(m, in, out);
    } else {
        cpu_error_marker(m, __FILE__, __LINE__);
        cpu_error_int(m, "opcode 0x%02X not implemented", opcode_num);
    }
    //print_stack(m);
    //dump_cpu(m);
    //cpu_instruction_context(m);
}

