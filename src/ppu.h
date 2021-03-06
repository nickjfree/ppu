#ifndef __PPU__
#define __PPU__


#include <stdint.h>
#include <stdlib.h>
#include "vram.h"
#include "renderer.h"
#include <iostream>
#include <bitset>
#include <vector>
 
using namespace std;

/**
    registers


    Name        Address Bits        Notes
    PPUCTRL     $2000   VPHB SINN   NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B), sprite tile select (S), increment mode (I), nametable select (NN)
    PPUMASK     $2001   BGRs bMmG   color emphasis (BGR), sprite enable (s), background enable (b), sprite left column enable (M), background left column enable (m), greyscale (G)
    PPUSTATUS   $2002   VSO- ----   vblank (V), sprite 0 hit (S), sprite overflow (O); read resets write pair for $2005/$2006
    OAMADDR     $2003   aaaa aaaa   OAM read/write address
    OAMDATA     $2004   dddd dddd   OAM data read/write
    PPUSCROLL   $2005   xxxx xxxx   fine scroll position (two writes: X scroll, Y scroll)
    PPUADDR     $2006   aaaa aaaa   PPU read/write address (two writes: most significant byte, least significant byte)
    PPUDATA     $2007   dddd dddd   PPU data read/write
    OAMDMA      $4014   aaaa aaaa   OAM DMA high address 
**/


/**
    scanlines defines    
**/

#define VISIBLE_LINE_START 0
#define VISIBLE_LINE_END   239
#define POST_LINE_START    240
#define POST_LINE_END      260
#define PRE_RENDER_LINE    261

#define ALL_VISIBLE_LINES  240
#define ALL_LINES          262
#define LINE_CYCLES        341

#define VBLANK_SET_LINE    241
#define VBLANK_CLEAR_LINE  261
#define VBLANK_SET_CYCLE     1
#define VBLANK_CLEAR_CYCLE   1    




class ppu 
{
private:
    /* video memery */

    vram memory;

    /* scanline renderer */ 
    renderer tv;

    /* mapped registers */

    uint8_t ppu_ctl;
    uint8_t ppu_mask;
    uint8_t ppu_status;
    uint8_t oam_addr;
    uint8_t oam_data;
    uint8_t ppu_scroll;
    uint16_t ppu_addr;
    uint8_t ppu_data;
    uint8_t oam_dma;

    /* nmi flag */
    bool nmi;

    /* internal registers */

    // current address
    uint16_t v;
    // temporary VRAM address (15 bits); can also be thought of as the address of the top left onscreen tile.
    uint16_t t;
    // First or second write toggle (1 bit)
    uint8_t w;
    // Fine X scroll (3 bits)
    uint8_t x;
    // prefetched shift register for background data, (NT0, AT0, sprite0_l, sprite0_r, NT1, AT1, sprite1_f, sprite1_r) 
    uint64_t fetched_color;
    // 8 sprite register
    uint32_t fetched_sprite[8];
    // current background color id;
    uint8_t background_color;
    // current sprite color id;
    uint8_t sprite_color;
    // final frame buffer
    std::vector<uint8_t> frame_buffer_;

    /* cycles */
    // current frame
    int frame;
    // mid-scanline cycle number
    int current_cycle;
    // mid-frame line number
    int current_scanline;

private:
    // update all the registers
    void update_registers();
    // increment hori(v)
    void inch();
    // increment vert(v)
    void incv();
    // hori(v) = hori(t)
    void copyht();
    // vert(v) = vert(t)
    void copyvt();
    // fetch nt
    uint8_t fetchnt();
    // fetch at
    uint8_t fetchat();
    // fetch sprite l
    uint8_t fetchspl(uint8_t index);
    // fetch sprite r
    uint8_t fetchspr(uint8_t index);
    // fetch  tile
    void fetch();
    // render current background color_id.
    uint8_t render_background_px();
    // render current sprite color_id.
    uint8_t render_sprite_px();
    // mixe backgroun and sprite color. and send to renderer
    void mix();
    // find sprite to render for next scanline
    void fetchsprites();
    // clear sprite register
    void clearsprite();

public:
    // reset ppu
    void reset();
    // power up
    void power_up();
    // shutdown 
    void shutdown();
    // update one cycle
    void tick();
    // update 
    void update(int cycles);
    // handle window message and update
    bool handle_window();
    /* interfaces for the cpu */

    void write_ppu_ctrl(uint8_t value);

    void write_ppu_mask(uint8_t value);

    void write_oam_addr(uint8_t value);

    void write_oam_data(uint8_t value);
    
    void write_ppu_scroll(uint8_t value);

    void write_ppu_addr(uint8_t value);

    void write_ppu_data(uint8_t value);

    void write_oam_dma(uint8_t value);

    uint8_t read_ppu_status();

    uint8_t read_oam_data();

    uint8_t read_ppu_data();
};








#endif

