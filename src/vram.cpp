#include "vram.h"



vram::vram() {
}

vram::~vram() {
}


// get value from vram
uint8_t vram::get_vram(uint16_t addr) {
    uint8_t * real = real_addr(addr);   
    return *real;
}

// get value from oam
uint8_t vram::get_oam(uint16_t addr) {
    return oam[addr];
}

// set value to vram
void vram::set_vram(uint16_t addr, uint8_t value) {
    uint8_t * real = real_addr(addr);
    *real = value;
}

// set value to oam
void vram::set_oam(uint16_t addr, uint8_t value) {
    oam[addr] = value;
}

// convert from nes address to real address
uint8_t * vram::real_addr(uint16_t addr) {
    if (addr >= 0x3f00) {
        // in palette range
        addr = 0x3f00 + (addr - 0x3f00) % PALETTE_SZIE;
        return (uint8_t*)palettes + addr;
    } else if (addr >= 0x2000) {
        // in name tables range
        addr = 0x2000 + (addr - 0x2000) % NAME_TABLE_SIZE;
        return (uint8_t*)name_tables + addr;
    } else if (addr >= 0) {
        // pattern table range
        addr = addr % PATTERN_TABLE_SIZE;
        return (uint8_t*)pattern_tables + addr;
    }
}
