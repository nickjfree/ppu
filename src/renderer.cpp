#include "renderer.h"


renderer::renderer(): width_(256), height_(240), debug_(BGFX_DEBUG_TEXT), reset_(BGFX_RESET_VSYNC) {
}

// init
void renderer::init() {

    
    bgfx::Init init;
    init.type     = bgfx::RendererType::OpenGL;
    init.vendorId = BGFX_PCI_ID_NONE;
    init.resolution.width  = width_;
    init.resolution.height = height_;
    init.resolution.reset = reset_;

    bgfx::init(init);
    bgfx::reset(width_, height_, reset_);
    bgfx::setDebug(debug_);
    bgfx::setViewClear(
        0,
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        0xFFFFFFFF,
        1.0f,
        0
    );

}

// shutdown
void renderer::shutdown() {

    bgfx::shutdown();

}

// update
bool renderer::update() {

    if(!entry::processEvents(width_, height_, debug_, reset_)) {
        bgfx::setViewRect(0, 0, 0, width_, height_);

        bgfx::touch(0);

        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 0, 0x4F, "Hello, world!");
        bgfx::dbgTextPrintf(0, 1, 0x4F, "width = %d, height = %d", width_, height_);
        bgfx::frame();
        return true;
    }
    return false;
}