#include "ppu.h"


inline bool ppu::is_rendering() {
    return (current_scanline >= VISIBLE_LINE_START && current_scanline <= VISIBLE_LINE_END);
}


ppu::ppu() {
    memory = new vram();
    reset();    
}


ppu::~ppu() {
    
    if (memory) {
        delete memory;
    }    
}


// reset ppu
void ppu::reset() {
    // reset all cycles
    current_scanline = PRE_RENDER_LINE;
    current_cycle = -1;
    frame = 0;
    nmi = false;

    // set registers on reset

    /*
                            At Power    After Reset
        PPUCTRL ($2000)     0000 0000   0000 0000
        PPUMASK ($2001)     0000 0000   0000 0000
        PPUSTATUS ($2002)   +0+x xxxx   U??x xxxx
        OAMADDR ($2003)     $00     unchanged1
        $2005 / $2006 latch     cleared     cleared
        PPUSCROLL ($2005)   $0000   $0000
        PPUADDR ($2006)     $0000   unchanged
        PPUDATA ($2007) read buffer     $00     $00
        odd frame   no  no
        OAM     unspecified     unspecified
        Palette     unspecified     unchanged
        NT RAM (external, in Control Deck)  unspecified     unchanged
        CHR RAM (external, in Game Pak)     unspecified     unchanged 
    */

    // just set them all to zero for now
    ppu_ctl = 0;
    ppu_mask = 0;
    ppu_status = 0 ;
    oam_addr = 0;
    oam_data = 0;
    ppu_scroll = 0 ;
    ppu_addr = 0;
    ppu_data = 0;
    oam_dma  = 0;

    // set all internal registers to 0
    v = t = w = x = 0;
}


void ppu::update_registers() {
    // set vblank 
    if (current_scanline == VBLANK_SET_LINE && current_cycle == VBLANK_SET_CYCLE) {
        ppu_status |= 0x80;
    }
    // clear vblank
    if (current_scanline == VBLANK_CLEAR_LINE && current_cycle == VBLANK_CLEAR_CYCLE) {
        ppu_status &= 0x7f;
    }

}


// update one cycle. must be called after all memory accessing
void ppu::tick() {


    int prev_scanline = current_scanline;
    int prev_cycle = current_cycle;

    // increment cycle

    current_cycle++;

    if (current_cycle > (LINE_CYCLES - 1)) {
        // this line is over, submit prev scanline
        // get x/y scroll then submit to renderer
        // renderer->scanline(number, xscroll, yscroll)
        current_cycle = current_cycle % LINE_CYCLES;
        current_scanline += 1;
    }

    // current frame is over
    if (current_scanline > PRE_RENDER_LINE) {
        current_scanline = current_scanline % ALL_LINES;

        // tell the renreder to draw one frame
        // renderer->draw_frame();

        frame++;
    }

    // update all registers
    update_registers();
}



void ppu::write_ppu_ctrl(uint8_t value) {
    ppu_ctl = value;
}

void ppu::write_ppu_mask(uint8_t value) {
    ppu_mask = value;
}

void ppu::write_oam_addr(uint8_t value) {
    oam_addr = value;
}

void ppu::write_oam_data(uint8_t value) {
    memory->set_oam(oam_addr, value);
    // increment oam_addr after write 
    oam_addr++;        
}

void ppu::write_ppu_scroll(uint8_t value) {
    if(w == 0) {
        // x scroll
        x = value & 0x0008;
        t = (value >> 3) & 0x001f;
        w = 1;
    } else {
        // y scroll
        t |= (value & 0x0008) << 12;
        t |= (value & 0x00f8) << 3; 
        w = 0;
    }
}

void ppu::write_ppu_addr(uint8_t value) {
    if (w == 0) {
        t |= (0x00ff &value) << 8;
        // clear 15th bit, bit 14
        t &= 0x3fff;
        w = 1;
    } else {
        t |= (0x00ff & value);
        v = t;
        w = 0;
    }
}

void ppu::write_ppu_data(uint8_t value) {
    memory->set_vram(v, value);
    if (ppu_ctl & 0x4) {
        v += 1;
    } else {
        v += 32;
    }
 }

void ppu::write_oam_dma(uint8_t value) {

}

uint8_t ppu::read_ppu_status() {
    uint8_t status = ppu_status;
    ppu_status &= 0x7f;
    return status;
}

uint8_t ppu::read_oam_data() {
    return memory->get_oam(oam_addr);
}

uint8_t ppu::read_ppu_data() {
    return memory->get_vram(v);
}

void ppu::update(int cycles) {
    while(cycles--) {
        tick();
    }
}