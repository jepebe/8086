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
            return (ReadOperand) {.word = (u16) mem->ram[addr]};
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
            cpu_error(cpu, "Unhandled write Memory Mode Table 1 '%d'", mem_mode);
        }
    }
}

//WriteOperand get_write_memory_byte(CPU *cpu, Memory *mem, MemoryModeTable1 mem_mode) {
//    char * err_msg = "unknown Byte Memory Mode Table1 '%s'";
//    switch (mem_mode) {
//        case BX_SI_PTR:cpu_error_str(cpu, mem, err_msg, "BX_SI_PTR");
//        case BX_DI_PTR:cpu_error_str(cpu, mem, err_msg, "BX_DI_PTR");
//        case BP_SI_PTR:cpu_error_str(cpu, mem, err_msg, "BP_SI_PTR");
//        case BP_DI_PTR:cpu_error_str(cpu, mem, err_msg, "BP_DI_PTR");
//        case SI_PTR:cpu_error_str(cpu, mem, err_msg, "SI_PTR");
//        case DI_PTR:cpu_error_str(cpu, mem, err_msg, "DI_PTR");
//        case DIRECT: {
//            cpu_error_str(cpu, mem, err_msg, "DIRECT");
//        }
//        case BX_PTR:cpu_error_str(cpu, mem, err_msg, "BX_PTR");
//        default:cpu_error_int(cpu, mem,"unknown Memory Mode Table 1 '%d'", mem_mode);
//    }
//}

char *decode_aoc(CPU *cpu, AddressOperandCode aoc) {
    static_assert(AOC_COUNT == 16, "Exhaustive number of AddressOperandCodes!");
    switch (aoc) {
        case Implied:
            return "";
        case IA:
            return "IA=0x%04X";
        case IB:
            return "IB=0x%02X";
        case IW:
            return "IW=0x%04X";
        case R_AX:
            return "AX";
        case R_BX:
            return "BX";
        case R_CX:
            return "CX";
        case R_DX:
            return "DX";
        case R_DH:
            return "DH";
        case R_BP:
            return "BP";
        case R_SP:
            return "SP";
        case R_SI:
            return "SI";
        case RW: {
            REG reg = (0x1 << 3) | cpu->addr_mode.reg_sreg;
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
        case SP_PTR:
            return ""; // Implied pointer to SP

        default: {
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_error(cpu, "Unhandled AddressOperandCode mode '%d'", aoc);
        }
    }
}
