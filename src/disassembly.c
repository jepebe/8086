#include "disassembly.h"
#include <stdio.h>

static char const *segment_prefix_as_str(SegmentOverride segment_override) {
    switch (segment_override) {
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

static void print_memory_mnemonic(MemoryMnemonic mm, Operand *op, char const *prefix) {
    if (mm.type != MMT_EMPTY || mm.name[0] != '\0') {
        if ((mm.type & MMT_POINTER) == MMT_POINTER)
            printf("[");

        if ((mm.type & MMT_SEGMENT_OVERRIDABLE) == MMT_SEGMENT_OVERRIDABLE)
            printf("%s", prefix);

        printf("%s", mm.name);

        if ((mm.type & (MMT_DISPLACEMENT_BYTE | MMT_DISPLACEMENT_WORD)) > 0) {
            if (op->displacement >= 0) {
                printf("+%d", op->displacement);
            } else {
                printf("%d", op->displacement);
            }
        }

        if ((mm.type & MMT_VALUE_WORD) == MMT_VALUE_WORD)
            printf("0x%04X", op->word_cache);

        if ((mm.type & MMT_VALUE_BYTE) == MMT_VALUE_BYTE)
            printf("0x%02X", op->byte_cache);

        if ((mm.type & MMT_ADDRESS) == MMT_ADDRESS)
            printf("%04X", op->word_cache);

        if ((mm.type & MMT_ADDRESS_32BIT) == MMT_ADDRESS_32BIT) {
            printf("%04X:", op->dword_cache >> 16);
            printf("%04X", op->dword_cache & 0x0000FFFF);
        }

        if ((mm.type & MMT_POINTER) == MMT_POINTER)
            printf("]");
    }
}

static MemoryMnemonic get_register_name(REG reg) {
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

static MemoryMnemonic get_segment_register_name(SegREG reg) {
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

static MemoryMnemonic get_memory_mode_name(MemoryModeTable mode, DisplacementType disp_type) {
    MemoryMnemonicType default_type = MMT_POINTER | MMT_SEGMENT_OVERRIDABLE;
    MemoryMnemonicType default_value_type;
    if (disp_type == WORD_DISP) {
        default_value_type = MMT_DISPLACEMENT_WORD;
    } else {
        default_value_type = MMT_DISPLACEMENT_BYTE;
    }

    switch (mode) {
        case BX_SI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "BX+SI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "BX+SI"};
        case BX_DI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "BX+DI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "BX+DI"};
        case BP_SI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "BP+SI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "BP+SI"};
        case BP_DI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "BP+DI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "BP+DI"};
        case SI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "SI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "SI"};
        case DI_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "DI"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "DI"};
        case DIRECT_OR_BP_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type | MMT_ADDRESS, "$"};
            } else {
                return (MemoryMnemonic) {default_type | default_value_type, "BP"};
            }
        case BX_PTR:
            if (disp_type == NO_DISP) {
                return (MemoryMnemonic) {default_type, "BX"};
            }
            return (MemoryMnemonic) {default_type | default_value_type, "BX"};
        default:
            return (MemoryMnemonic) {.name ="Unknown Memory Mode"};
    }
}

static const MemoryMnemonic AOC_TABLE[AOC_COUNT] = {
        [Implied] = {MMT_EMPTY, ""},
        [ONE] = {.name ="1"},
        [IA] = {MMT_POINTER | MMT_ADDRESS | MMT_SEGMENT_OVERRIDABLE, "$"},
        [IB] = {MMT_VALUE_BYTE, ""},
        [IB_SE] = {MMT_VALUE_BYTE, "(SE) "},
        [IW] = {MMT_VALUE_WORD, ""},
        [IDW] = {MMT_ADDRESS_32BIT, "$"},
        [R_AX] = {.name ="AX"},
        [R_AL] = {.name ="AL"},
        [R_AH] = {.name ="AH"},
        [R_BX] = {.name ="BX"},
        [R_BL] = {.name ="BL"},
        [R_CX] = {.name ="CX"},
        [R_CL] = {.name ="CL"},
        [R_CH] = {.name ="CH"},
        [R_DX] = {.name ="DX"},
        [R_DL] = {.name ="DL"},
        [R_DH] = {.name ="DH"},
        [R_BP] = {.name ="BP"},
        [R_SP] = {.name ="SP"},
        [R_SI] = {.name ="SI"},
        [R_DI] = {.name ="DI"},
};

static MemoryMnemonic decode_aoc(Machine *machine, AddressOperandCode aoc) {
    CPU *cpu = machine->cpu;

    MemoryMnemonic repr = AOC_TABLE[aoc];
    if (repr.name) {
        return repr;
    }
    switch (aoc) {
        case RB:
        case RW: {
            bool word = false;
            if (aoc == RW) {
                word = true;
            }
            REG reg = (word << 3) | cpu->addr_mode.reg_sreg;
            return get_register_name(reg);
        }
        case RMB:
        case RMW: {
            bool word = false;
            if (aoc == RMW) {
                word = true;
            }
            u8 mode = cpu->addr_mode.mode;
            if (mode == 0) {
                MemoryModeTable mem_mode = cpu->addr_mode.reg_mem;
                return get_memory_mode_name(mem_mode, NO_DISP);

            } else if (mode == 1) {
                MemoryModeTable mem_mode = cpu->addr_mode.reg_mem;
                return get_memory_mode_name(mem_mode, BYTE_DISP);

            } else if (mode == 2) {
                MemoryModeTable mem_mode = cpu->addr_mode.reg_mem;
                return get_memory_mode_name(mem_mode, WORD_DISP);

            } else if (mode == 3) {
                REG reg = (word << 3) | cpu->addr_mode.reg_mem;
                return get_register_name(reg);
            }
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_note(machine, "Unsupported mode %d", mode);
            cpu_error(machine, "Unhandled RM%c mode", (word ? 'W' : 'B'));
        }
        case SR: {
            SegREG reg = cpu->addr_mode.reg_sreg;
            return get_segment_register_name(reg);
        }

        default: {
            cpu_error_marker(machine, __FILE__, __LINE__);
            cpu_error(machine, "Unhandled AddressOperandCode mode '%d'", aoc);
        }
    }
}

void disassemble_instruction(Machine *machine, u32 addr, u8 opc, Opcode opcode, Operand *out, Operand *in) {
    printf("[$%05X] ", addr);
    printf("[%02X] ", opc); // Opcode number

    //print_cpu(machine);

    if (machine->cpu->repeat != NO_REPEAT) {
        switch (machine->cpu->repeat) {
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

    print_memory_mnemonic(w, out, prefix);

    MemoryMnemonic r = decode_aoc(machine, opcode.read_op);

    if (w.type != MMT_EMPTY && r.type != MMT_EMPTY) {
        printf(", ");
    }

    print_memory_mnemonic(r, in, prefix);

    printf("\n");
}
