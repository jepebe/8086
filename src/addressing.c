#include "addressing.h"
#include "debug.h"

void fetch_addressing_mode(Machine *m) {
    m->cpu->addr_mode.opcode = m->memory->ram[cpu_ip(m->cpu)];
    m->cpu->IP++;
}

AddrMode peek_addressing_mode(Machine *machine) {
    return (AddrMode) {.opcode = machine->memory->ram[cpu_ip(machine->cpu)]};
}

ReadOperand get_read_register(Machine *machine, REG reg) {
    CPU *cpu = machine->cpu;
    switch (reg) {
        case AL:
            return (ReadOperand) {.byte = cpu->AL};
        case CL:
            return (ReadOperand) {.byte = cpu->CL};
        case DL:
            return (ReadOperand) {.byte = cpu->DL};
        case BL:
            return (ReadOperand) {.byte = cpu->BL};
        case AH:
            return (ReadOperand) {.byte = cpu->AH};
        case CH:
            return (ReadOperand) {.byte = cpu->CH};
        case DH:
            return (ReadOperand) {.byte = cpu->DH};
        case BH:
            return (ReadOperand) {.byte = cpu->BH};
        case AX:
            return (ReadOperand) {.word = cpu->AX};
        case CX:
            return (ReadOperand) {.word = cpu->CX};
        case DX:
            return (ReadOperand) {.word = cpu->DX};
        case BX:
            return (ReadOperand) {.word = cpu->BX};
        case SP:
            return (ReadOperand) {.word = cpu->SP};
        case BP:
            return (ReadOperand) {.word = cpu->BP};
        case SI:
            return (ReadOperand) {.word = cpu->SI};
        case DI:
            return (ReadOperand) {.word = cpu->DI};
        default:
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unknown register type '%d'", reg);
    }
}

WriteOperand get_write_register(Machine *machine, REG reg) {
    CPU *cpu = machine->cpu;
    switch (reg) {
        case AL:
            return (WriteOperand) {.byte = &cpu->AL};
        case CL:
            return (WriteOperand) {.byte = &cpu->CL};
        case DL:
            return (WriteOperand) {.byte = &cpu->DL};
        case BL:
            return (WriteOperand) {.byte = &cpu->BL};
        case AH:
            return (WriteOperand) {.byte = &cpu->AH};
        case CH:
            return (WriteOperand) {.byte = &cpu->CH};
        case DH:
            return (WriteOperand) {.byte = &cpu->DH};
        case BH:
            return (WriteOperand) {.byte = &cpu->BH};
        case AX:
            return (WriteOperand) {.word = &cpu->AX};
        case CX:
            return (WriteOperand) {.word = &cpu->CX};
        case DX:
            return (WriteOperand) {.word = &cpu->DX};
        case BX:
            return (WriteOperand) {.word = &cpu->BX};
        case SP:
            return (WriteOperand) {.word = &cpu->SP};
        case BP:
            return (WriteOperand) {.word = &cpu->BP};
        case SI:
            return (WriteOperand) {.word = &cpu->SI};
        case DI:
            return (WriteOperand) {.word = &cpu->DI};
        default:
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unknown register type '%d'", reg);
    }
}

WriteOperand get_write_segment_register(Machine *machine, SegREG reg) {
    CPU *cpu = machine->cpu;
    switch (reg) {
        case ES:
            return (WriteOperand) {.word = &cpu->ES};
        case CS:
            return (WriteOperand) {.word = &cpu->CS};
        case SS:
            return (WriteOperand) {.word = &cpu->SS};
        case DS:
            return (WriteOperand) {.word = &cpu->DS};
        default:
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unknown segment register type '%d'", reg);
    }
}

ReadOperand get_read_segment_register(Machine *machine, SegREG reg) {
    CPU *cpu = machine->cpu;
    switch (reg) {
        case ES:
            return (ReadOperand) {.word = cpu->ES};
        case CS:
            return (ReadOperand) {.word = cpu->CS};
        case SS:
            return (ReadOperand) {.word = cpu->SS};
        case DS:
            return (ReadOperand) {.word = cpu->DS};
        default:
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unknown segment register type '%d'", reg);
    }
}

static u16 get_displacement(Machine *machine, MemoryMode mode) {
    CPU *cpu = machine->cpu;
    Memory *mem = machine->memory;

    u16 displacement = 0;
    switch (mode.displacement_type) {
        case NO_DISP: {
            if (mode.memory_mode == DIRECT_OR_BP_PTR) {
                displacement = read_memory_u16(cpu_ip(cpu), mem);
                cpu->IP += 2;
            }
            break;
        }
        case BYTE_DISP: {
            displacement = read_memory_u8(cpu_ip(cpu), mem);
            cpu->IP += 1;
            break;
        }
        case WORD_DISP: {
            displacement = read_memory_u16(cpu_ip(cpu), mem);
            cpu->IP += 2;
            break;
        }
        default:
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unknown displacement type %d", mode.displacement_type);
    }
    return displacement;
}

static u32 displace_address(Machine *machine, MemoryMode mode, u16 displacement) {
    CPU *cpu = machine->cpu;
    u32 addr;
    switch (mode.memory_mode) {
        case DIRECT_OR_BP_PTR: {
            if (mode.displacement_type == NO_DISP) {
                // direct
                addr = cpu_ds(cpu, displacement);
            } else {
                // [BP+disp]
                addr = cpu_ds(cpu, cpu->BP + displacement);
            }
            break;
        }
        case SI_PTR: {
            addr = cpu_ds(cpu, cpu->SI + displacement);
            break;
        }
        case DI_PTR: {
            addr = cpu_ds(cpu, cpu->DI + displacement);
            break;
        }
        case BX_SI_PTR:
        case BX_DI_PTR:
        case BP_SI_PTR:
        case BP_DI_PTR:
        case BX_PTR:
        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unhandled Memory Mode Table mode '%d'", mode.memory_mode);
        }
    }
    return addr;
}

ReadOperand get_read_memory(Machine *m, MemoryMode mode) {
    Memory *mem = m->memory;

    u16 displacement = get_displacement(m, mode);
    m->cpu->immediate_read = displacement;
    u32 addr = displace_address(m, mode, displacement);

    switch (mode.operand_size) {
        case BYTE:
            return (ReadOperand) {.byte = *(u8 *) &mem->ram[addr]};
        case WORD:
            return (ReadOperand) {.word = *(u16 *) &mem->ram[addr]};
        default: {
            cpu_error_marker(m, __FILE__, __LINE__);
            cpu_error(m, "Unknown operand size '%d'", mode.operand_size);
        }
    }
}

WriteOperand get_write_memory(Machine *m, MemoryMode mode) {
    Memory *mem = m->memory;
    u16 displacement = get_displacement(m, mode);
    m->cpu->immediate_write = displacement;
    u32 addr = displace_address(m, mode, displacement);

    switch (mode.operand_size) {
        case BYTE:
            return (WriteOperand) {.byte = (u8 *) &mem->ram[addr]};
        case WORD:
            return (WriteOperand) {.word = (u16 *) &mem->ram[addr]};
        default: {
            cpu_error_marker(m, __FILE__, __LINE__);
            cpu_error(m, "Unknown operand size '%d'", mode.operand_size);
        }
    }
}

char *get_register_name(REG reg) {
    switch (reg) {
        case AL:
            return "AL";
        case CL:
            return "CL";
        case DL:
            return "DL";
        case BL:
            return "BL";
        case AH:
            return "AH";
        case CH:
            return "CH";
        case DH:
            return "DH";
        case BH:
            return "BH";
        case AX:
            return "AX";
        case CX:
            return "CX";
        case DX:
            return "DX";
        case BX:
            return "BX";
        case SP:
            return "SP";
        case BP:
            return "BP";
        case SI:
            return "SI";
        case DI:
            return "DI";
        default:
            return "Unknown register";
    }
}

char *get_segment_register_name(SegREG reg) {
    switch (reg) {
        case ES:
            return "ES";
        case CS:
            return "CS";
        case SS:
            return "SS";
        case DS:
            return "DS";
        default:
            return "Unknown Segment Register";
    }
}

char *get_memory_mode_table_name(MemoryModeTable mode, DisplacementType disp_type) {
    switch (mode) {
        case BX_SI_PTR:
            return "[BX+SI]";
        case BX_DI_PTR:
            return "[BX+DI]";
        case BP_SI_PTR:
            return "[BP+SI]";
        case BP_DI_PTR:
            return "[BP+DI]";
        case SI_PTR:
            return "[SI+0x%02X]";
        case DI_PTR:
            return "[DI+0x%02X]";
        case DIRECT_OR_BP_PTR:
            if (disp_type == NO_DISP) {
                return "IDA=0x%04X";
            } else {
                return "[BP+0x%02X]";
            }
        case BX_PTR:
            return "[BX]";
        default:
            return "Unknown Memory Mode Table1";
    }
}
