#include "ppu.h"

// reset ppu
void ppu::reset() {
    // reset all cycles
    current_scanline = PRE_RENDER_LINE;
    current_cycle = -1;
    frame = 0;
    nmi = false;
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


// power up
void ppu::power_up() {
    reset();
    tv.init(&memory);
}


// shutdown 
void ppu::shutdown() {
    tv.shutdown();    
}


// increment hori(v)
void ppu::inch() {
    if ((v & 0x001f) == 31) {
       v &= 0xFFE0;
       v ^= 0x0400;
    } else {
       v++;
    }
}

// increment vert(v)
void ppu::incv() {
    int finey = v & 0x7000;
    finey += 0x1000;
    if (finey == 0x8000) {
        // clear finey
        v = v & 0x0fff;
        // increment coarse
        int coarsey = (v & 0x03e0) >> 5;
        if (coarsey == 29) {
            coarsey = 0;
            // increment to next table
            v = v ^ 0x0800;            
        } else if (coarsey == 31) {
            // Coarse Y can be set out of bounds
            coarsey = 0;
        } else {
            coarsey++;
        }
        v = (v & 0xfc1f) | (coarsey << 5);       
    } else {
        v = (v & 0x0fff) | finey;
    }
}

// hori(v) = hori(t)
void ppu::copyht() {
    v = (v & 0xFBE0) | (t & 0x041F);
}

void ppu::copyvt() {
    v = (v & 0x841F) | (t & 0x7BE0);
}

// fetch nt
uint8_t ppu::fetchnt() {
    uint16_t addr = 0x2000 | (v & 0x0FFF);
    return memory.get_vram(addr);
}

// fetch at
uint8_t ppu::fetchat() {
    uint16_t addr = 0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
    int shift = ((v >> 4) & 4) | (v & 2);
    return (memory.get_vram(addr) >> shift) & 0x0003;
}

// fetch sprite l
uint8_t ppu::fetchspl(uint8_t index) {
    uint16_t finey = (v >> 12) & 0x0007;
    uint16_t addr =  ((ppu_ctl & 0x0010) << 8) + index * 16 + finey;
    return memory.get_vram(addr);
}

// fetch sprite r
uint8_t ppu::fetchspr(uint8_t index) {
    uint16_t finey = (v >> 12) & 0x0007;
    uint16_t addr =  ((ppu_ctl & 0x0010) << 8) + index * 16 + finey + 8;
    return memory.get_vram(addr);
}

void ppu::fetch() {
    uint8_t nt = fetchnt();
    uint8_t at = fetchat();
    uint8_t sp_l = fetchspl(nt);
    uint8_t sp_r = fetchspr(nt);

    // generate attribute bits to the shift register
    uint64_t data = 0;
    for (int i = 0; i < 8; i++) {
        // right sprite table has higher bit
        int shift = 7 - i;
        uint8_t color_id = (at << 2) | ((sp_r >> shift) & 0x01) << 1 | ((sp_l >> shift) & 0x01);
        data <<= 4;
        data |= (color_id & 0x0f);
    }
    fetched_color |= data;
    //cout << "fecther: " << bitset<64>(fetched_color) << "pt: " << current_cycle << " " << current_scanline << endl;}
}

// render current background color_id.
uint8_t ppu::render_background_px() {
    uint8_t finex = x;
    uint32_t color_data = fetched_color >> 32;
    int shift = (7 - finex) * 4;
    uint8_t color_id = (color_data >> shift) & 0x000f;  
    fetched_color = fetched_color << 4; 
    //cout << "render : " << bitset<64>(fetched_color) << "pt: " << current_cycle << " " << current_scanline << endl;
    return color_id; 
}

// render current sprite color_id.
uint8_t ppu::render_sprite_px() {
    return 0;
}

// mixe backgroun and sprite color. and send to renderer
void ppu::mix() {

}

void ppu::update_registers() {

    // set flags
    bool preline = current_scanline == 261;
    bool postline = current_scanline == 240;
    bool vblankset = current_scanline == 241;
    bool visible = current_scanline >=0 && current_scanline <= 239;
    bool vblank = current_scanline >=240 && current_scanline <= 260;
    bool fetching = visible || preline;
    // set rendering mask
    bool show_background = ppu_mask & 0x08;
    bool show_sprite = ppu_mask & 0x10;
    bool show_left_background = ppu_mask & 0x02;
    bool show_lelf_sprite = ppu_mask & 0x04;

    bool enabled = show_background || show_sprite;

    bool fetch_cycle = (current_cycle >= 0 && current_cycle <= 256) || (current_cycle >= 321 && current_cycle <= 336);

    bool render_cycle = (current_cycle >= 1 && current_cycle <= 256);

    bool prefetch_render = (current_cycle >= 231 && current_cycle <= 336);

    bool setup_cycle = (current_cycle >= 280 && current_cycle <= 304);
    // if rendering is enabled
    if (enabled) {
        // are we in ppu fetching cycles
        if(fetching) {
            // do the  rendering
            if (current_cycle && (render_cycle || prefetch_render)) {
                int index = current_cycle % 8;
                // draw a background pixel
                uint8_t bg_color = render_background_px();
                // draw a sprite pixel
                uint8_t sp_color = render_sprite_px();
                // update framebuffer
                if (visible && render_cycle) {
                    frame_buffer_.push_back(bg_color); 
                }
            }
            // emulate fetching operations, cycle 0 is idle 
            if (current_cycle && fetch_cycle) {
                int index = current_cycle % 8;
                if (index == 0) {
                    // fetch next tile
                    fetch();
                    // go to next
                    inch();
                }  
                if (current_cycle == 256) {
                    incv();
                }
            } 
            // hori(v) = hori(t)
            if (current_cycle == 257) {
                copyht();
            }
        }
        // are we in pre-renderline and setup stage
        if (preline && setup_cycle) {
            // vert(v) = vert(t)
            copyvt(); 
        }
    }
    // debug  output 
    // if (v & 0x0c00) {
    //     cout << "v: " << bitset<16>(v) << " t: "<< bitset<16>(t) << " " << current_scanline << "\t" <<  current_cycle << " " << frame << endl;
    // }
    // set vblank (241, 1)    
    if (vblankset && current_cycle == 1) {
        ppu_status |= 0x80;
    }
    // clear vblank at (261, 1)
    if (preline && current_cycle == 1) {
        ppu_status &= 0x7f;
    }
}


// update one cycle. must be called after all memory accessing
void ppu::tick() {


    int prev_scanline = current_scanline;
    int prev_cycle = current_cycle;

    // increment cycle

    current_cycle++;

    // skip one cycle if at odd frame and at pre-render line
    if(current_cycle == 339 && current_scanline == PRE_RENDER_LINE && frame % 2) {
        current_cycle++;
    } 


    if (current_cycle > 340 ) {
        // this line is over
        current_cycle = current_cycle % LINE_CYCLES;
        // std::cout << "line: " << current_scanline << endl;
        current_scanline += 1;
    }
    // current frame is over
    if (current_scanline > PRE_RENDER_LINE) {
        current_scanline = current_scanline % ALL_LINES;
        if (frame_buffer_.size()) {
            for (int i = 0; i < 240; i++) {
                for (int j = 0; j < 256; j++) {
                   cout << int(frame_buffer_[i*256+j]&3);
                }
                cout << endl;
            } 
        }
        // flush all scanlines
        tv.submit(frame_buffer_.data(), 61440);
        // tell the renreder to draw one frame
        tv.frame();
        std::cout << endl << "frame: " << frame << "size:" << frame_buffer_.size()<< endl;
        frame_buffer_.clear();
        frame++;
    }

    // update all registers
    update_registers();
}


void ppu::write_ppu_ctrl(uint8_t value) {
    ppu_ctl = value;
    // set high bit of X/Y scroll. base name table high bit
    t = (t & 0xf3ff) | ((uint16_t(value) & 0x03) << 10);
}

void ppu::write_ppu_mask(uint8_t value) {
    ppu_mask = value;
}

void ppu::write_oam_addr(uint8_t value) {
    oam_addr = value;
}

void ppu::write_oam_data(uint8_t value) {
    memory.set_oam(oam_addr, value);
    // increment oam_addr after write 
    oam_addr++;        
}

void ppu::write_ppu_scroll(uint8_t value) {
    if(w == 0) {
        // x scroll
        x = value & 0x0008;
        t &= 0xffe0;
        t |= (value >> 3) & 0x001f;
        w = 1;
    } else {
        // y scroll
        t &= 0x0c1f;
        t |= (value & 0x0008) << 12;
        t |= (value & 0x00f8) << 3; 
        w = 0;
    }
}

void ppu::write_ppu_addr(uint8_t value) {
    if (w == 0) {
        t &= 0x00ff;
        t |= (0x00ff &value) << 8;
        // clear 15th bit, bit 14
        t &= 0x3fff;
        w = 1;
    } else {
        t &= 0xff00;
        t |= 0x00ff & value;
        v = t;
        w = 0;
    }
}

void ppu::write_ppu_data(uint8_t value) {
    memory.set_vram(v, value);
    if (!(ppu_ctl & 0x4)) {
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
    // read status also set w to 0
    w = 0;
    return status;
}

uint8_t ppu::read_oam_data() {
    return memory.get_oam(oam_addr);
}

uint8_t ppu::read_ppu_data() {
    return memory.get_vram(v);
}

void ppu::update(int cycles) {
    while(cycles--) {
        tick();
    }
}

bool ppu::handle_window() {
    return tv.update();
}