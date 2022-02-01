#pragma once

#include "cpu.h"

ReadOperand get_read_register(CPU *cpu, REG reg) {
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
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_error(cpu, "Unknown register type '%d'", reg);
    }
}

WriteOperand get_write_register(CPU *cpu, REG reg) {
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
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_error(cpu, "Unknown register type '%d'", reg);
    }
}

WriteOperand get_write_segment_register(CPU *cpu, SegREG reg) {
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
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_error(cpu, "Unknown segment register type '%d'", reg);
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

typedef enum {
    /// [BX+SI]
    BX_SI_PTR = 0x00,
    BX_DI_PTR = 0x01,
    BP_SI_PTR = 0x02,
    BP_DI_PTR = 0x03,
    SI_PTR = 0x04,
    DI_PTR = 0x05,
    DIRECT = 0x06,
    BX_PTR = 0x07
} MemoryModeTable1;

char *get_memory_mode_table1_name(MemoryModeTable1 mode) {
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
            return "[SI]";
        case DI_PTR:
            return "[DI]";
        case DIRECT:
            return "IDA=0x%04X";
        case BX_PTR:
            return "[BX]";
        default:
            return "Unknown Memory Mode Table1";
    }
}

ReadOperand get_read_memory(CPU *cpu, Memory *mem, MemoryModeTable1 mem_mode) {
    switch (mem_mode) {
        case DIRECT: {
            u16 offset = read_memory_u16(cpu_ip(cpu), mem);
            cpu->immediate_read = offset;
            cpu->IP += 2;
            u32 addr = cpu_ds(cpu, offset);
            return (ReadOperand) {.word = *(u16 *) &mem->ram[addr]};
        }
        case BX_SI_PTR:
        case BX_DI_PTR:
        case BP_SI_PTR:
        case BP_DI_PTR:
        case SI_PTR:
        case DI_PTR:
        case BX_PTR:
        default: {
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_error(cpu, "Unhandled write Memory Mode Table 1 '%d'", mem_mode);
        }
    }
}

WriteOperand get_write_memory(CPU *cpu, Memory *mem, MemoryModeTable1 mem_mode, OperandSize op_size) {
    switch (mem_mode) {
        case DIRECT: {
            u16 offset = read_memory_u16(cpu_ip(cpu), mem);
            cpu->immediate_write = offset;
            cpu->IP += 2;
            u32 addr = cpu_ds(cpu, offset);
            if (op_size == WORD) {
                return (WriteOperand) {.word = (u16 *) &mem->ram[addr]};
            } else {
                return (WriteOperand) {.byte = (u8 *) &mem->ram[addr]};
            }
        }
        case BX_SI_PTR:
        case BX_DI_PTR:
        case BP_SI_PTR:
        case BP_DI_PTR:
        case SI_PTR:
        case DI_PTR:
        case BX_PTR:
        default: {
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_error_int(cpu, mem, "Unhandled write Memory Mode Table 1 '%d'", mem_mode);
        }
    }
}

const char *AOC_TABLE[] = {
        [Implied] = "",
        [ONE] = "1",
        [IA] = "IA=0x%04X",
        [IB] = "IB=0x%02X",
        [IB_SE] = "IBSE=0x%02X",
        [IW] = "IW=0x%04X",
        [R_AX] = "AX",
        [R_AL] = "AL",
        [R_AH] = "AH",
        [R_BX] = "BX",
        [R_BL] = "BL",
        [R_CX] = "CX",
        [R_CL] = "CL",
        [R_CH] = "CH",
        [R_DX] = "DX",
        [R_DL] = "DL",
        [R_DH] = "DH",
        [R_BP] = "BP",
        [R_SP] = "SP",
        [R_SI] = "SI",
        [R_DI] = "DI",
        [SP_PTR] ="", // Implied pointer to SP
};

const char *decode_aoc(CPU *cpu, AddressOperandCode aoc) {
    //static_assert(AOC_COUNT == 19, "Exhaustive number of AddressOperandCodes!");
    const char *repr = AOC_TABLE[aoc];
    if (repr) {
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
                MemoryModeTable1 mem_mode = cpu->addr_mode.reg_mem;
                return get_memory_mode_table1_name(mem_mode);

            } else if (mode == 3) {
                REG reg = (word << 3) | cpu->addr_mode.reg_mem;
                return get_register_name(reg);
            }
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_note_int(cpu, "Unsupported mode %d", mode);
            cpu_error(cpu, "Unhandled RM%c mode", (word ? 'W' : 'B'));
        }
        case SR: {
            SegREG reg = cpu->addr_mode.reg_sreg;
            return get_segment_register_name(reg);
        }

        default: {
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_error(cpu, "Unhandled AddressOperandCode mode '%d'", aoc);
        }
    }
}
