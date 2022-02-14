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

static char const *segment_prefix_as_str(SegmentOverride segment_override) {
    switch(segment_override) {
        case CS_SEGMENT:
            return "CS:";
        case DS_SEGMENT:
            return "DS:";
        case ES_SEGMENT:
            return "ES:";
        case SS_SEGMENT:
            return "SS:";
        default:
            return "";
    }
}

static void disassemble_instruction(Machine *machine, u32 addr, u8 opc, Opcode opcode) {
    printf("[$%05X] ", addr);
    printf("[%02X] ", opc); // Opcode number

    if(machine->cpu->repeat != NO_REPEAT) {
        switch(machine->cpu->repeat) {
            case REP:
                printf("REP ");
                break;
            case REPZ:
                printf("REPZ ");
                break;
            case REPNZ:
                printf("REPNZ ");
                break;
            default:
                break;
        }
    }

    printf("%s ", opcode.name); // opcode mnemonic

    MemoryMnemonic w = decode_aoc(machine, opcode.write_op);

    char const *prefix = segment_prefix_as_str(machine->cpu->segment_override);

    if (w.name[0] != '\0') {
        if((w.type & MMT_POINTER) == MMT_POINTER)
            printf("[");

        if((w.type & MMT_SEGMENT_OVERRIDABLE) == MMT_SEGMENT_OVERRIDABLE)
            printf("%s", prefix);

        printf("%s", w.name);

        if((w.type & MMT_VALUE_WORD) == MMT_VALUE_WORD)
            printf("0x%04X",  machine->cpu->immediate_write.word);

        if((w.type & MMT_VALUE_BYTE) == MMT_VALUE_BYTE)
            printf("0x%02X",  machine->cpu->immediate_write.byte);

        if((w.type & MMT_ADDRESS) == MMT_ADDRESS)
            printf("%04X",  machine->cpu->immediate_write.word);

        if((w.type & MMT_POINTER) == MMT_POINTER)
            printf("]");
    }

    MemoryMnemonic r = decode_aoc(machine, opcode.read_op);

    if (w.name[0] != '\0' && r.name[0] != '\0') {
        printf(", ");
    }

    if (r.name[0] != '\0') {
        if((r.type & MMT_POINTER) == MMT_POINTER)
            printf("[");

        if((r.type & MMT_SEGMENT_OVERRIDABLE) == MMT_SEGMENT_OVERRIDABLE)
            printf("%s", prefix);

        printf("%s", r.name);

        if((r.type & MMT_DISPLACEMENT_WORD) == MMT_DISPLACEMENT_WORD)
            printf("%d",  machine->cpu->displacement);

        if((r.type & MMT_DISPLACEMENT_BYTE) == MMT_DISPLACEMENT_BYTE)
            printf("%d",  machine->cpu->displacement);

        if((r.type & MMT_VALUE_WORD) == MMT_VALUE_WORD)
            printf("0x%04X",  machine->cpu->immediate_read.word);

        if((r.type & MMT_VALUE_BYTE) == MMT_VALUE_BYTE)
            printf("0x%02X",  machine->cpu->immediate_read.word);

        if((r.type & MMT_ADDRESS) == MMT_ADDRESS)
            printf("%04X",  machine->cpu->immediate_read.word);

        if((r.type & MMT_POINTER) == MMT_POINTER)
            printf("]");
    }

    printf("\n");
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
        prefix_opcode.op_fn(m, (ReadOperand) {0}, (WriteOperand) {0});
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
        WriteOperand out = decode_write_op(m, opcode.write_op);
        ReadOperand in = decode_read_op(m, opcode.read_op);

        opcode.op_fn(m, in, out);

        disassemble_instruction(m, addr, opcode_num, opcode);
    } else {
        cpu_error_marker(m, __FILE__, __LINE__);
        cpu_error_int(m, "opcode 0x%02X not implemented", opcode_num);
    }
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

