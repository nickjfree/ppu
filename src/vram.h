#ifndef __VRAM__
#define __VRAM__

/** 
    Address range   Size    Description
    $0000-$0FFF     $1000   Pattern table 0
    $1000-$1FFF     $1000   Pattern Table 1
    $2000-$23FF     $0400   Nametable 0
    $2400-$27FF     $0400   Nametable 1
    $2800-$2BFF     $0400   Nametable 2
    $2C00-$2FFF     $0400   Nametable 3
    $3000-$3EFF     $0F00   Mirrors of $2000-$2EFF
    $3F00-$3F1F     $0020   Palette RAM indexes
    $3F20-$3FFF     $00E0   Mirrors of $3F00-$3F1F 
**/

#include <stdint.h>
#include "renderer.h"


#define PATTERN_TABLE_SIZE  (0x1000)
#define NAME_TABLE_SIZE     (0x0400)
#define PALETTE_SZIE        (0x0020)
#define OAM_SIZE            (0x0100)   

/**
    ppu vram
**/
class vram 
{
private:
    // pattern tables 
    uint8_t pattern_tables[2][PATTERN_TABLE_SIZE]; 
    // name tables
    uint8_t name_tables[4][NAME_TABLE_SIZE];
    // palette 
    uint8_t palettes[PALETTE_SZIE];
    // internal oam memery
    uint8_t oam[OAM_SIZE];

private:
    // convert from nes address to real address
    uint8_t * real_addr(uint16_t addr);

public:
    vram();
    ~vram();
    // get value from vram
    uint8_t get_vram(uint16_t addr);
    // get oam data
    uint8_t get_oam(uint16_t addr);
    // set value to vram
    void set_vram(uint16_t addr, uint8_t value);
    // set value to oam
    void set_oam(uint16_t addr, uint8_t value);
};

#endif
