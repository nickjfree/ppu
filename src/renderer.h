#ifndef __RENDERER__
#define __RENDERER__

#include "bgfx/bgfx.h" 
#include "common/common.h"

class renderer 
{

private:
    uint32_t width_;
    uint32_t height_;
    uint32_t debug_;
    uint32_t reset_;

public:
    renderer();
    // init
    void init();
    // shutdown
    void shutdown();
    // update
    bool update();

};







#endif