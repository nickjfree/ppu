
#include <iostream>
#include "ppu.h"
#include "chr_test.h"

using namespace std;


int _main_(int argc, char *argv[]) {

    ppu nes_ppu = ppu();
    nes_ppu.power_up();

    // write some test data to memory
    // use pattern table 2 as background
    nes_ppu.write_ppu_ctrl(0x90);    
    // write a pattern data
    nes_ppu.write_ppu_addr(0x00);
    nes_ppu.write_ppu_addr(0x00);

    // write test pattern data
    int chr_index = 0;
    while(chr_index < 8192) {
        nes_ppu.write_ppu_data(chr_mario[chr_index++]);
    }

    nes_ppu.write_ppu_addr(0x20);
    nes_ppu.write_ppu_addr(0x00);
    int count = 32*30;
    int background_index = 0;
    while(count--) {
        nes_ppu.write_ppu_data(background[background_index++]);  
    }
    // write attribute data
    count = 64;
    int attr_index = 0;
    while(count--) {
        nes_ppu.write_ppu_data(attr[attr_index++]);
        // nes_ppu.write_ppu_data(0);
    }
    // write palette data
    int palette_index = 0;
    count = 32;

    nes_ppu.write_ppu_addr(0x3f);
    nes_ppu.write_ppu_addr(0x00);

    while(count--) {
        nes_ppu.write_ppu_data(palette[palette_index++]);
        // nes_ppu.write_ppu_data(0);
    }


    // set x y scroll
    nes_ppu.write_ppu_scroll(0);
    nes_ppu.write_ppu_scroll(0);
    // set flags
    nes_ppu.write_ppu_ctrl(0x90); 
    nes_ppu.write_ppu_mask(0x1e);

    do {
        // do some simulations each loop
        nes_ppu.update(10000);
    } while(nes_ppu.handle_window());

    // shutdown 
    nes_ppu.shutdown();
    return 0;
}


