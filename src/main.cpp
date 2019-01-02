
#include <iostream>
#include "ppu.h"


using namespace std;

int _main_(int argc, char *argv[]) {
    renderer * test_renderer = new renderer();
    test_renderer->init();
    while(test_renderer->update());
    test_renderer->shutdown();
    return 0;
}


