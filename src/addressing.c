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
            return (ReadOperand) {.byte = &cpu->AL};
        case CL:
            return (ReadOperand) {.byte = &cpu->CL};
        case DL:
            return (ReadOperand) {.byte = &cpu->DL};
        case BL:
            return (ReadOperand) {.byte = &cpu->BL};
        case AH:
            return (ReadOperand) {.byte = &cpu->AH};
        case CH:
            return (ReadOperand) {.byte = &cpu->CH};
        case DH:
            return (ReadOperand) {.byte = &cpu->DH};
        case BH:
            return (ReadOperand) {.byte = &cpu->BH};
        case AX:
            return (ReadOperand) {.word = &cpu->AX};
        case CX:
            return (ReadOperand) {.word = &cpu->CX};
        case DX:
            return (ReadOperand) {.word = &cpu->DX};
        case BX:
            return (ReadOperand) {.word = &cpu->BX};
        case SP:
            return (ReadOperand) {.word = &cpu->SP};
        case BP:
            return (ReadOperand) {.word = &cpu->BP};
        case SI:
            return (ReadOperand) {.word = &cpu->SI};
        case DI:
            return (ReadOperand) {.word = &cpu->DI};
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
            return (ReadOperand) {.word = &cpu->ES};
        case CS:
            return (ReadOperand) {.word = &cpu->CS};
        case SS:
            return (ReadOperand) {.word = &cpu->SS};
        case DS:
            return (ReadOperand) {.word = &cpu->DS};
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
            displacement = (s16) sign_extend_byte( read_memory_u8(cpu_ip(cpu), mem));
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
        case BX_DI_PTR:
        case BX_PTR:
        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unhandled Memory Mode Table mode '%d'", mode.memory_mode);
        }
    }
    return effective_addr(machine, offset, default_segment);
}

ReadOperand get_read_memory(Machine *m, MemoryMode mode) {
    Memory *mem = m->memory;

    s16 displacement = get_displacement(m, mode);
    m->cpu->displacement = displacement;
    u32 addr = displace_address(m, mode, displacement);

    switch (mode.operand_size) {
        case BYTE:
            return (ReadOperand) {.byte = (u8 *) &mem->ram[addr], .addr = addr};
        case WORD:
            return (ReadOperand) {.word = (u16 *) &mem->ram[addr], .addr = addr};
        default: {
            cpu_error_marker(m, __FILE__, __LINE__);
            cpu_error(m, "Unknown operand size '%d'", mode.operand_size);
        }
    }
}

WriteOperand get_write_memory(Machine *m, MemoryMode mode) {
    Memory *mem = m->memory;
    s16 displacement = get_displacement(m, mode);
    m->cpu->immediate_write.word = displacement;
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

MemoryMnemonic get_register_name(REG reg) {
    switch (reg) {
        case AL:
            return (MemoryMnemonic) {.name="AL"};
        case CL:
            return (MemoryMnemonic) {.name="CL"};
        case DL:
            return (MemoryMnemonic) {.name="DL"};
        case BL:
            return (MemoryMnemonic) {.name="BL"};
        case AH:
            return (MemoryMnemonic) {.name="AH"};
        case CH:
            return (MemoryMnemonic) {.name="CH"};
        case DH:
            return (MemoryMnemonic) {.name="DH"};
        case BH:
            return (MemoryMnemonic) {.name="BH"};
        case AX:
            return (MemoryMnemonic) {.name="AX"};
        case CX:
            return (MemoryMnemonic) {.name="CX"};
        case DX:
            return (MemoryMnemonic) {.name="DX"};
        case BX:
            return (MemoryMnemonic) {.name="BX"};
        case SP:
            return (MemoryMnemonic) {.name="SP"};
        case BP:
            return (MemoryMnemonic) {.name="BP"};
        case SI:
            return (MemoryMnemonic) {.name="SI"};
        case DI:
            return (MemoryMnemonic) {.name="DI"};
        default:
            return (MemoryMnemonic) {.name="Unknown register"};
    }
}

MemoryMnemonic get_segment_register_name(SegREG reg) {
    switch (reg) {
        case ES:
            return (MemoryMnemonic) {.name = "ES"};
        case CS:
            return (MemoryMnemonic) {.name = "CS"};
        case SS:
            return (MemoryMnemonic) {.name = "SS"};
        case DS:
            return (MemoryMnemonic) {.name = "DS"};
        default:
            return (MemoryMnemonic) {.name = "Unknown Segment Register"};
    }
}

MemoryMnemonic get_memory_mode_name(MemoryModeTable mode, DisplacementType disp_type) {
    MemoryMnemonicType default_type = MMT_POINTER | MMT_SEGMENT_OVERRIDABLE;
    MemoryMnemonicType default_value_type;
    if(disp_type == WORD_DISP) {
        default_value_type = MMT_DISPLACEMENT_WORD;
    } else {
        default_value_type = MMT_DISPLACEMENT_BYTE;
    }

    switch (mode) {
        case BX_SI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "BX+SI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "BX+SI+"};
        case BX_DI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "BX+DI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "BX+DI+"};
        case BP_SI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "BP+SI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "BP+SI+"};
        case BP_DI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "BP+DI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "BP+DI+"};
        case SI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "SI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "SI+"};
        case DI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "DI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "DI+"};
        case DIRECT_OR_BP_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type | MMT_ADDRESS, "$"};
            } else {
                return (MemoryMnemonic) {default_type | default_value_type, "BP+"};
            }
        case BX_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "BX"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "BX+"};
        default:
            return (MemoryMnemonic) {.name ="Unknown Memory Mode"};
    }
}
