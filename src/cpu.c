#include "cpu.h"
#include "addressing.h"
#include "opcodes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void dump_ram(CPU *cpu, Memory *mem) {
    (void) cpu;
    for (int i = 0; i < 0x100; i += 16) {
        printf("$%04X ", i);
        for (int j = 0; j < 16; ++j) {
            printf("%02x ", mem->ram[i + j] & 0xFF);
        }
        printf("\n");
    }
}

void dump_cpu(CPU *cpu, Memory *memory) {
    (void) memory;
    printf("[SS:SP] = $%04X:%04X ", cpu->SS, cpu->SP);
    printf("[CS:IP] = $%04X:%04X\n", cpu->CS, cpu->IP);
    printf("[AX] = 0x%04X ", cpu->AX);
    printf("[BX] = 0x%04X ", cpu->BX);
    printf("[CX] = 0x%04X ", cpu->CX);
    printf("[DX] = 0x%04X\n", cpu->DX);
}

void cpu_instruction_context(CPU *cpu, Memory *mem) {
    u32 segment = cpu_ip(cpu) & 0xFFFF0;
    printf("\x1b[0;31m[$%05X] ", segment);
    for (int i = 0; i < 16; ++i) {
        u32 addr = segment + i;
        printf("%02X ", mem->ram[addr]);
    }
    printf("\x1b[0m\n");
}

NO_RETURN void cpu_error(CPU *cpu, char *message, int value) {
    printf("\x1b[0;31m");
    printf("[$%05X] Error: ", cpu_ip(cpu));
    printf(message, value);
    printf("\x1b[0m\n");

    exit(1);
}

void cpu_note_int(CPU *cpu, char *message, int value) {
    printf("\x1b[0;31m");
    printf("[$%05X] Error: ", cpu_ip(cpu));
    printf(message, value);
    printf("\x1b[0m\n");
}

void cpu_error_marker(CPU *cpu, char *file, int line) {
    printf("\x1b[0;31m");
    printf("[$%05X] Error: %s:%d", cpu_ip(cpu), file, line);
    printf("\x1b[0m\n");
}

void cpu_note_str(CPU *cpu, char *message, char *value) {
    printf("\x1b[0;31m");
    printf("[$%05X] Error: ", cpu_ip(cpu));
    printf(message, value);
    printf("\x1b[0m\n");
}

void cpu_peek(CPU *cpu, Memory *mem) {
    u32 segment = cpu_ip(cpu) & 0xFFFF0;
    printf("\x1b[0;31m[$%05X] ", segment);
    for (int i = 0; i < 16; ++i) {
        u32 addr = segment + i;
        printf("%02X ", mem->ram[addr]);
    }
    printf("\x1b[0m\n");
}

char *flags_to_str(Flags flags) {
    char *res = malloc(sizeof(char) * 10);
    res[9] = '\0';
    res[8] = flags.CF ? 'c' : '.';
    res[7] = flags.PF ? 'p' : '.';
    res[6] = flags.AF ? 'a' : '.';
    res[5] = flags.ZF ? 'z' : '.';
    res[4] = flags.SF ? 's' : '.';
    res[3] = flags.TF ? 't' : '.';
    res[2] = flags.IF ? 'i' : '.';
    res[1] = flags.DF ? 'd' : '.';
    res[0] = flags.OF ? 'o' : '.';
    return res;
}

NO_RETURN void cpu_error_int(CPU *cpu, Memory *memory, char *message, int value) {
    printf("\x1b[0;31m");
    printf("[$%05X] Error: ", cpu_ip(cpu));
    printf(message, value);
    printf("\n");
    cpu_instruction_context(cpu, memory);
    exit(1);
}

NO_RETURN void cpu_error_str(CPU *cpu, Memory *mem, char *message, char *value) {
    printf("\x1b[0;31m");
    printf("[$%05X] Error: ", cpu_ip(cpu));
    printf(message, value);
    printf("\n");
    cpu_instruction_context(cpu, mem);
    exit(1);
}

u16 read_memory_u16(u32 addr, Memory *memory) {
    u8 lo = memory->ram[addr];
    u8 hi = memory->ram[addr + 1];
    return (hi << 8) | lo;
}

u8 read_memory_u8(u32 addr, Memory *memory) {
    return memory->ram[addr];
}


ReadOperand read(CPU *cpu, Memory *mem, AddressOperandCode read_op) {
    //static_assert(AOC_COUNT == 19, "Exhaustive number of AddressOperandCodes!");
    switch (read_op) {
        case Implied: {
            return (ReadOperand) {0};
        }
        case ONE: {
            return (ReadOperand) {.word = 1};
        }
        case IB: {
            u8 b = read_memory_u8(cpu_ip(cpu), mem);
            cpu->immediate_read = b;
            cpu->IP += 1;
            return (ReadOperand) {.byte = b};
        }
        case IB_SE: {
            u8 lo_byte = read_memory_u8(cpu_ip(cpu), mem);
            u8 hi_byte = ((lo_byte & 0x80) == 0x80) ? 0xFF : 0x00;

            u16 word = (hi_byte << 8) | lo_byte;
            cpu->immediate_read = word;
            cpu->IP += 1;
            return (ReadOperand) {.word = word};
        }
        case IW: {
            u16 w = read_memory_u16(cpu_ip(cpu), mem);
            cpu->immediate_read = w;
            cpu->IP += 2;
            return (ReadOperand) {.word = w};
        }
        case R_AX: {
            return (ReadOperand) {.word = cpu->AX};
        }
        case R_AL: {
            return (ReadOperand) {.word = cpu->AL};
        }
        case R_CX: {
            return (ReadOperand) {.word = cpu->CX};
        }
        case R_CL: {
            return (ReadOperand) {.word = cpu->CL};
        }
        case R_DX: {
            return (ReadOperand) {.word = cpu->DX};
        }
        case SP_PTR: {
            u16 top = read_memory_u16(cpu_sp(cpu), mem);
            return (ReadOperand) {.word = top};
        }
        case RB:
        case RW: {
            OperandSize op_size = BYTE;
            if (read_op == RW) {
                op_size = WORD;
            }
            REG reg = (op_size << 3) | cpu->addr_mode.reg_sreg;
            return get_read_register(cpu, reg);
        }
        case RMB:
        case RMW: {
            OperandSize op_size = BYTE;
            if (read_op == RMW) {
                op_size = WORD;
            }
            u8 mode = cpu->addr_mode.mode;
            if (mode == 0) {
                //printf("ModR/M %d\n", cpu->addr_mode.opcode);
                MemoryModeTable1 mem_mode = cpu->addr_mode.reg_mem;
                return get_read_memory(cpu, mem, mem_mode);

            } else if (mode == 3) {
                REG reg = (op_size << 3) | cpu->addr_mode.reg_mem;
                return get_read_register(cpu, reg);
            }
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_note_int(cpu, "Unsupported mode %d", mode);
            cpu_error(cpu, "Unhandled read RM%c mode", (op_size ? 'W' : 'B'));
        }

        default: {
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_error_int(cpu, mem, "unsupported read operation '%d'!", read_op);
        }
    }
}

WriteOperand write(CPU *cpu, Memory *mem, AddressOperandCode write_op) {
    (void) mem;
    //static_assert(AOC_COUNT == 19, "Exhaustive number of AddressOperandCodes!");
    switch (write_op) {
        case Implied: {
            return (WriteOperand) {0};
        }
        case IA: {
            u16 offset = read_memory_u16(cpu_ip(cpu), mem);
            cpu->immediate_write = offset;
            cpu->IP += 2;
            u32 addr = cpu_ds(cpu, offset);
            return (WriteOperand) {.word = (uint16_t *) &mem->ram[addr]};
        }
        case R_AX: {
            return (WriteOperand) {.word =  &cpu->AX};
        }
        case R_AL: {
            return (WriteOperand) {.byte =  &cpu->AL};
        }
        case R_AH: {
            return (WriteOperand) {.byte =  &cpu->AH};
        }
        case R_BX: {
            return (WriteOperand) {.word =  &cpu->BX};
        }
        case R_BL: {
            return (WriteOperand) {.byte =  &cpu->BL};
        }
        case R_CX: {
            return (WriteOperand) {.word =  &cpu->CX};
        }
        case R_CL: {
            return (WriteOperand) {.byte =  &cpu->CL};
        }
        case R_CH: {
            return (WriteOperand) {.byte =  &cpu->CH};
        }
        case R_DX: {
            return (WriteOperand) {.word =  &cpu->DX};
        }
        case R_DH: {
            return (WriteOperand) {.byte =  &cpu->DH};
        }
        case R_DL: {
            return (WriteOperand) {.byte =  &cpu->DL};
        }
        case R_BP: {
            return (WriteOperand) {.word =  &cpu->BP};
        }
        case R_SP: {
            return (WriteOperand) {.word =  &cpu->SP};
        }
        case R_SI: {
            return (WriteOperand) {.word =  &cpu->SI};
        }
        case R_DI: {
            return (WriteOperand) {.word =  &cpu->DI};
        }
        case RMB:
        case RMW: {
            OperandSize op_size = BYTE;
            if (write_op == RMW) {
                op_size = WORD;
            }
            u8 mode = cpu->addr_mode.mode;
            if (mode == 0) {
                MemoryModeTable1 mem_mode = cpu->addr_mode.reg_mem;
                return get_write_memory(cpu, mem, mem_mode, op_size);

            } else if (mode == 3) {
                REG reg = (op_size << 3) | cpu->addr_mode.reg_mem;
                return get_write_register(cpu, reg);
            }
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_note_int(cpu, "Unsupported mode %d", mode);
            cpu_error(cpu, "Unhandled write RM%c mode", (op_size ? 'W' : 'B'));

            //cpu_note_int(cpu, "Unsupported mode %d", mode);
            //cpu_error_str(cpu, mem, "Unhandled RM%c mode", (op_size ? "W" : "B"));
        }
        case RB:
        case RW: {
            OperandSize op_size = BYTE;
            if (write_op == RW) {
                op_size = WORD;
            }
            REG reg = (op_size << 3) | cpu->addr_mode.reg_sreg;
            // printf("reg = %s [%d]\n", get_register_name(reg), reg);
            return get_write_register(cpu, reg);
        }
        case SR: {
            SegREG reg = cpu->addr_mode.reg_sreg;
            return get_write_segment_register(cpu, reg);
        }
        case SP_PTR: {
            u32 sp = cpu_sp(cpu);
            return (WriteOperand) {.word =  (u16 *) &mem->ram[sp]};
        }
        default: {
            cpu_error_marker(cpu, __FILE__, __LINE__);
            cpu_error_int(cpu, mem, "unsupported write operation '%d'!", write_op);
        }
    }
}


static void disassemble_instruction(CPU *cpu, u32 addr, u8 opc, Opcode opcode) {
    printf("[$%05X] ", addr);
    printf("[%02X] ", opc); // Opcode number
    printf("%s ", opcode.name); // opcode mnemonic

    const char *w = decode_aoc(cpu, opcode.write_op);

    if (w[0] != '\0') {
        if (strchr(w, '%') != NULL) {
            printf(w, cpu->immediate_write);
        } else {
            printf("%s", w);
        }
    }

    const char *r = decode_aoc(cpu, opcode.read_op);

    if (w[0] != '\0' && r[0] != '\0') {
        printf(", ");
    }

    if (r[0] != '\n') {
        if (strchr(r, '%') != NULL) {
            printf(r, cpu->immediate_read);
        } else {
            printf("%s", r);
        }
    }

    printf("\n");
}


void cpu_tick(CPU *cpu, Memory *memory) {
    u32 addr = cpu_ip(cpu);
    u8 opcode_num = memory->ram[addr];
    cpu->IP++;
    cpu->opcode = opcode_num;
    Opcode opcode = fetch_opcode(cpu, memory, opcode_num);
    if (opcode.op_fn != NULL) {
        if (has_mod_rm_byte(opcode)) {
            fetch_addressing_mode(cpu, memory);
        }
        WriteOperand out = write(cpu, memory, opcode.write_op);
        ReadOperand in = read(cpu, memory, opcode.read_op);
        opcode.op_fn(cpu, in, out);

        disassemble_instruction(cpu, addr, opcode_num, opcode);

    } else {
        cpu_error_marker(cpu, __FILE__, __LINE__);
        cpu_error_int(cpu, memory, "opcode 0x%02X not implemented", opcode_num);
    }
}

void cpu_reset(CPU *cpu) {
    cpu->CS = 0xFFFF;
    cpu->DS = 0x0000;
    cpu->SS = 0x0000;
    cpu->IP = 0x0000;
    cpu->halted = false;
    cpu->flags.word = 0x0002; // set first unused bit to 1
}

u32 cpu_ip(CPU *cpu) {
    // this code handles 16 bit wrap around
    u32 hi_seg = ((cpu->CS & 0xF000) << 4);
    u16 lo_seg = (cpu->CS & 0x0FFF) << 4;
    return hi_seg + ((lo_seg + cpu->IP) & 0xFFFF);
}

u32 cpu_ds(CPU *cpu, u16 offset) {
    return (cpu->DS << 4) + offset;
}

u32 cpu_sp(CPU *cpu) {
    return (cpu->SS << 4) + cpu->SP;
}



