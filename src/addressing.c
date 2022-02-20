#include "addressing.h"
#include "debug.h"

void fetch_addressing_mode(Machine *m) {
    m->cpu->addr_mode.opcode = m->memory->ram[cpu_ip(m->cpu)];
    m->cpu->IP++;
}

AddrMode peek_addressing_mode(Machine *machine) {
    return (AddrMode) {.opcode = machine->memory->ram[cpu_ip(machine->cpu)]};
}

void decode_register(Machine *machine, REG reg, Operand *op) {
    CPU *cpu = machine->cpu;
    switch (reg) {
        case AL:
            op->byte = &cpu->AL;
            break;
        case CL:
            op->byte = &cpu->CL;
            break;
        case DL:
            op->byte = &cpu->DL;
            break;
        case BL:
            op->byte = &cpu->BL;
            break;
        case AH:
            op->byte = &cpu->AH;
            break;
        case CH:
            op->byte = &cpu->CH;
            break;
        case DH:
            op->byte = &cpu->DH;
            break;
        case BH:
            op->byte = &cpu->BH;
            break;
        case AX:
            op->word = &cpu->AX;
            break;
        case CX:
            op->word = &cpu->CX;
            break;
        case DX:
            op->word = &cpu->DX;
            break;
        case BX:
            op->word = &cpu->BX;
            break;
        case SP:
            op->word = &cpu->SP;
            break;
        case BP:
            op->word = &cpu->BP;
            break;
        case SI:
            op->word = &cpu->SI;
            break;
        case DI:
            op->word = &cpu->DI;
            break;
        default:
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unknown register type '%d'", reg);
    }
}

void decode_segment_register(Machine *machine, SegREG reg, Operand *op) {
    CPU *cpu = machine->cpu;
    switch (reg) {
        case ES:
            op->word = &cpu->ES;
            break;
        case CS:
            op->word = &cpu->CS;
            break;
        case SS:
            op->word = &cpu->SS;
            break;
        case DS:
            op->word = &cpu->DS;
            break;
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

SegmentOverride default_displacement_segment(Machine *machine, MemoryMode mode) {
    SegmentOverride default_segment;
    switch (mode.memory_mode) {
        case DIRECT_OR_BP_PTR: {
            if (mode.displacement_type == NO_DISP) {
                // direct
                default_segment = DS_SEGMENT;
            } else {
                // [BP+disp]
                default_segment = SS_SEGMENT;
            }
            break;
        }
        case BP_SI_PTR:
        case BP_DI_PTR: {
            default_segment = SS_SEGMENT;
            break;
        }
        case DI_PTR:
        case SI_PTR:
        case BX_SI_PTR:
        case BX_PTR:
        case BX_DI_PTR: {
            default_segment = DS_SEGMENT;
            break;
        }
        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unhandled Memory Mode Table mode '%d'", mode.memory_mode);
        }
    }
    return default_segment;
}

static u16 displace_address(Machine *machine, MemoryMode mode, s16 displacement) {
    CPU *cpu = machine->cpu;
    u16 offset;
    switch (mode.memory_mode) {
        case DIRECT_OR_BP_PTR: {
            if (mode.displacement_type == NO_DISP) {
                // direct
                offset = displacement;
            } else {
                // [BP+disp]
                offset = cpu->BP + displacement;
            }
            break;
        }
        case SI_PTR: {
            offset = cpu->SI + displacement;
            break;
        }
        case DI_PTR: {
            offset = cpu->DI + displacement;
            break;
        }
        case BP_SI_PTR: {
            offset = cpu->BP + cpu->SI + displacement;
            break;
        }
        case BP_DI_PTR: {
            offset = cpu->BP + cpu->DI + displacement;
            break;
        }
        case BX_SI_PTR: {
            offset = cpu->BX + cpu->SI + displacement;
            break;
        }
        case BX_PTR: {
            offset = cpu->BX + displacement;
            break;
        }
        case BX_DI_PTR: {
            offset = cpu->BX + cpu->DI + displacement;
            break;
        }
        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unhandled Memory Mode Table mode '%d'", mode.memory_mode);
        }
    }
    return offset;
}

void decode_operand(Machine *machine, MemoryMode mode, Operand *op) {
    Memory *mem = machine->memory;

    s16 displacement = get_displacement(machine, mode);
    u16 offset = displace_address(machine, mode, displacement);
    SegmentOverride default_segment = default_displacement_segment(machine, mode);
    u32 addr = effective_addr(machine, offset, default_segment);

    op->displacement = displacement;
    op->word_cache = offset;

    switch (mode.operand_size) {
        case BYTE:
            op->byte = (u8 *) &mem->ram[addr];
            break;
        case WORD:
            op->word = (u16 *) &mem->ram[addr];
            break;
        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unknown operand size '%d'", mode.operand_size);
        }
    }
}
