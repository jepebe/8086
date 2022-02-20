#include "addressing.h"
#include "debug.h"

void fetch_addressing_mode(Machine *m) {
    m->cpu->addr_mode.opcode = m->memory->ram[cpu_ip(m->cpu)];
    m->cpu->IP++;
}

AddrMode peek_addressing_mode(Machine *machine) {
    return (AddrMode) {.opcode = machine->memory->ram[cpu_ip(machine->cpu)]};
}

Operand get_register(Machine *machine, REG reg) {
    CPU *cpu = machine->cpu;
    switch (reg) {
        case AL:
            return (Operand) {.byte = &cpu->AL};
        case CL:
            return (Operand) {.byte = &cpu->CL};
        case DL:
            return (Operand) {.byte = &cpu->DL};
        case BL:
            return (Operand) {.byte = &cpu->BL};
        case AH:
            return (Operand) {.byte = &cpu->AH};
        case CH:
            return (Operand) {.byte = &cpu->CH};
        case DH:
            return (Operand) {.byte = &cpu->DH};
        case BH:
            return (Operand) {.byte = &cpu->BH};
        case AX:
            return (Operand) {.word = &cpu->AX};
        case CX:
            return (Operand) {.word = &cpu->CX};
        case DX:
            return (Operand) {.word = &cpu->DX};
        case BX:
            return (Operand) {.word = &cpu->BX};
        case SP:
            return (Operand) {.word = &cpu->SP};
        case BP:
            return (Operand) {.word = &cpu->BP};
        case SI:
            return (Operand) {.word = &cpu->SI};
        case DI:
            return (Operand) {.word = &cpu->DI};
        default:
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unknown register type '%d'", reg);
    }
}

Operand get_segment_register(Machine *machine, SegREG reg) {
    CPU *cpu = machine->cpu;
    switch (reg) {
        case ES:
            return (Operand) {.word = &cpu->ES};
        case CS:
            return (Operand) {.word = &cpu->CS};
        case SS:
            return (Operand) {.word = &cpu->SS};
        case DS:
            return (Operand) {.word = &cpu->DS};
        default:
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unknown segment register type '%d'", reg);
    }
}

static inline u16 sign_extend_byte(u8 byte) {
    return ((byte & 0x80) == 0x80) ? 0xFF00 | byte : byte;
}

static s16 get_displacement(Machine *machine, MemoryMode mode) {
    CPU *cpu = machine->cpu;
    Memory *mem = machine->memory;

    s16 displacement = 0;
    switch (mode.displacement_type) {
        case NO_DISP: {
            if (mode.memory_mode == DIRECT_OR_BP_PTR) {
                displacement = (s16) read_memory_u16(cpu_ip(cpu), mem);
                cpu->IP += 2;
            }
            break;
        }
        case BYTE_DISP: {
            displacement = (s16) sign_extend_byte(read_memory_u8(cpu_ip(cpu), mem));
            cpu->IP += 1;
            break;
        }
        case WORD_DISP: {
            displacement = (s16) read_memory_u16(cpu_ip(cpu), mem);
            cpu->IP += 2;
            break;
        }
        default:
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unknown displacement type %d", mode.displacement_type);
    }
    return displacement;
}

static u32 displace_address(Machine *machine, MemoryMode mode, s16 displacement) {
    CPU *cpu = machine->cpu;
    SegmentOverride default_segment;
    u16 offset;
    switch (mode.memory_mode) {
        case DIRECT_OR_BP_PTR: {
            if (mode.displacement_type == NO_DISP) {
                // direct
                default_segment = DS_SEGMENT;
                offset = displacement;
            } else {
                // [BP+disp]
                default_segment = SS_SEGMENT;
                offset = cpu->BP + displacement;
            }
            break;
        }
        case SI_PTR: {
            default_segment = DS_SEGMENT;
            offset = cpu->SI + displacement;
            break;
        }
        case DI_PTR: {
            default_segment = DS_SEGMENT;
            offset = cpu->DI + displacement;
            break;
        }
        case BP_SI_PTR: {
            default_segment = SS_SEGMENT;
            offset = cpu->BP + cpu->SI + displacement;
            break;
        }
        case BP_DI_PTR: {
            default_segment = SS_SEGMENT;
            offset = cpu->BP + cpu->DI + displacement;
            break;
        }
        case BX_SI_PTR: {
            default_segment = DS_SEGMENT;
            offset = cpu->BX + cpu->SI + displacement;
            break;
        }
        case BX_PTR: {
            default_segment = DS_SEGMENT;
            offset = cpu->BX + displacement;
            break;
        }
        case BX_DI_PTR: {
            default_segment = DS_SEGMENT;
            offset = cpu->BX + cpu->DI + displacement;
            break;
        }
        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unhandled Memory Mode Table mode '%d'", mode.memory_mode);
        }
    }
    return effective_addr(machine, offset, default_segment);
}

Operand get_operand(Machine *m, MemoryMode mode) {
    Memory *mem = m->memory;

    s16 displacement = get_displacement(m, mode);
    u32 addr = displace_address(m, mode, displacement);

    Operand op = {
            .displacement = displacement,
            .word_cache = displacement,
            .addr = addr
    };

    switch (mode.operand_size) {
        case BYTE:
            op.byte = (u8 *) &mem->ram[addr];
            break;
        case WORD:
            op.word = (u16 *) &mem->ram[addr];
            break;
        default: {
            cpu_error_marker(m, __FILE__, __LINE__);
            cpu_error(m, "Unknown operand size '%d'", mode.operand_size);
        }
    }
    return op;
}
