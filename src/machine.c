#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "machine.h"
#include "opcodes.h"
#include "debug.h"
#include "disassembly.h"

Machine machine_create() {
    CPU *cpu = malloc(sizeof(CPU));
    cpu_reset(cpu);
    Memory *mem = malloc(sizeof(Memory));

    Machine machine = {
            .cpu = cpu,
            .memory = mem,
            .IP = (Operand) {.word = &cpu->IP},
            .CS = (Operand) {.word = &cpu->CS},
    };
    return machine;
}

bool is_prefix_opcode(u8 opcode) {
    switch (opcode) {
        case 0x26:  // ES:
        case 0x2E:  // CS:
        case 0x36:  // SS:
        case 0xF2:  // REPNZ
        case 0xF3:  // REPZ
            return true;
        default:
            return false;
    }
}

void machine_tick(Machine *m) {
    m->cpu->segment_override = DEFAULT_SEGMENT;
    m->cpu->repeat = NO_REPEAT;

    u32 addr = cpu_ip(m->cpu);
    u8 opcode_num = m->memory->ram[addr];

    m->cpu->opcode_ip = m->cpu->IP;

    while (is_prefix_opcode(opcode_num)) {
        // run the prefix function before the actual function
        Opcode prefix_opcode = fetch_opcode(m, opcode_num);
        prefix_opcode.op_fn(m, NULL, NULL);
        m->cpu->IP++;
        addr = cpu_ip(m->cpu);
        opcode_num = m->memory->ram[addr];
    }

    m->cpu->IP++;
    m->cpu->opcode = opcode_num;

    Opcode opcode = fetch_opcode(m, opcode_num);

    if (opcode.op_fn != NULL) {
        if (has_mod_rm_byte(opcode)) {
            fetch_addressing_mode(m);
        }
        decode_write_op(m, opcode.write_op);
        decode_read_op(m, opcode.read_op);

        Operand *read_op = &m->cpu->read_op;
        Operand *write_op = &m->cpu->write_op;

        opcode.op_fn(m, read_op, write_op);

        disassemble_instruction(m, addr, opcode_num, opcode, write_op, read_op);
    } else {
        cpu_error_marker(m, __FILE__, __LINE__);
        cpu_error(m, "opcode 0x%02X not implemented", opcode_num);
    }
    //usleep(1000 * 50);
    //print_stack(m);
    //dump_cpu(m);
    //cpu_instruction_context(m);
}

u32 effective_addr(Machine *m, u16 offset, SegmentOverride default_segment) {
    if(default_segment == DEFAULT_SEGMENT) {
        cpu_error(m, "Default segment can't be DEFAULT_SEGMENT", 0);
    }
    SegmentOverride segment = m->cpu->segment_override;

    if (segment == DEFAULT_SEGMENT) {
        segment = default_segment;
    }

    switch(segment) {
        case CS_SEGMENT:
            return ((m->cpu->CS << 4) + offset) & 0xFFFFF;
        case DS_SEGMENT:
            return ((m->cpu->DS << 4) + offset) & 0xFFFFF;
        case ES_SEGMENT:
            return ((m->cpu->ES << 4) + offset) & 0xFFFFF;
        case SS_SEGMENT:
            return ((m->cpu->SS << 4) + offset) & 0xFFFFF;
        default:
            cpu_error(m, "Unhandled segment type: %d", segment);
    }
}

u32 effective_addr_no_override(Machine *m, u16 offset, SegmentOverride segment) {
    if(segment == DEFAULT_SEGMENT) {
        cpu_error(m, "Segment can't be DEFAULT_SEGMENT", 0);
    }

    switch(segment) {
        case CS_SEGMENT:
            return ((m->cpu->CS << 4) + offset) & 0xFFFFF;
        case DS_SEGMENT:
            return ((m->cpu->DS << 4) + offset) & 0xFFFFF;
        case ES_SEGMENT:
            return ((m->cpu->ES << 4) + offset) & 0xFFFFF;
        case SS_SEGMENT:
            return ((m->cpu->SS << 4) + offset) & 0xFFFFF;
        default:
            cpu_error(m, "Unhandled segment type: %d", segment);
    }
}
