#include "80x86.h"
#include "memory.h"

u16 read_memory_u16(u32 addr, Memory *memory) {
    u8 lo = memory->ram[addr];
    u8 hi = memory->ram[addr + 1];
    return (hi << 8) | lo;
}

u8 read_memory_u8(u32 addr, Memory *memory) {
    return memory->ram[addr];
}

void write_memory_u8(u32 addr, Memory *memory, u8 value) {
    memory->ram[addr] = value;
}

void write_memory_u16(u32 addr, Memory *memory, u16 value) {
    memory->ram[addr] = value & 0x00FF;
    memory->ram[addr + 1] = value >> 8;
}
