
#include <iostream>
#include "ppu.h"


using namespace std;


int _main_(int argc, char *argv[]) {

    ppu nes_ppu = ppu();
    nes_ppu.power_up();

    do {
        // do some simulations each loop
        nes_ppu.update(10000);
    } while(nes_ppu.handle_window());

    // shutdown 
    nes_ppu.shutdown();
    return 0;
}


