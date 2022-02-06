#pragma once

typedef struct {
    u8 ram[1024 * 1024];
} Memory;

u16 read_memory_u16(u32 addr, Memory *memory);

u8 read_memory_u8(u32 addr, Memory *memory);

void write_memory_u8(u32 addr, Memory *memory, u8 value);

void write_memory_u16(u32 addr, Memory *memory, u16 value);
