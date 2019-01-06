#include "renderer.h"

// vertext declar
bgfx::VertexDecl quad_vertex::vtx_decl;

// vertext data define
static quad_vertex s_quadVertices[4] =
{
    {-1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    {-1.0f, -1.0f,  1.0f},
    { 1.0f, -1.0f,  1.0f},
};


// index data define 
static const uint16_t s_quadIndices[6] =
{
    0, 1, 2,  
    2, 1, 3, 
};




renderer::renderer(): width_(1024), height_(960), debug_(BGFX_DEBUG_TEXT), reset_(BGFX_RESET_VSYNC) {
}



// create progeram
bgfx::ProgramHandle renderer::CreateProgram(const uint8_t * vs_code, uint32_t vs_size, const uint8_t * ps_code, uint32_t ps_size) {


    bgfx::ShaderHandle vs = bgfx::createShader(bgfx::makeRef(vs_code, vs_size));
    bgfx::ShaderHandle ps = bgfx::createShader(bgfx::makeRef(ps_code, ps_size));
    return bgfx::createProgram(vs, ps, false);
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
        BGFX_CLEAR_DEPTH|BGFX_CLEAR_STENCIL,
        0x000000FF,
        1.0f,
        0
    );

    // init vertex buffer decl
    quad_vertex::init();
    // init buffers and shaders
    vtx_line_ = bgfx::createVertexBuffer(
                      bgfx::makeRef(s_quadVertices, sizeof(s_quadVertices) )
                    , quad_vertex::vtx_decl
                    );

        // Create static index buffer.
    idx_line_ = bgfx::createIndexBuffer(
                bgfx::makeRef(s_quadIndices, sizeof(s_quadIndices) )
                );


    // Create program from shaders.
    program_scanline_ = CreateProgram(vs_scanline, sizeof(vs_scanline), ps_scanline, sizeof(ps_scanline));
}

// shutdown
void renderer::shutdown() {
    
    bgfx::destroy(vtx_line_);
    bgfx::destroy(idx_line_);
    bgfx::destroy(program_scanline_);
    bgfx::shutdown();

}

// update
bool renderer::update() {

    // present to screen
    uint32_t width;
    uint32_t height;

    if(!entry::processEvents(width, height, debug_, reset_)) {
        bgfx::setViewRect(0, 0, 0, width_, height_);
        bgfx::touch(0);       
        bgfx::frame();
        return true;
    }
    return false;
}


int renderer::scanline(int line_number, float xscroll, float yscroll) {
    // fill scanline cpu buffer
    if (line_number < 0 || line_number > 239) {
        // do not render vblank lines and pre-render line
        return -1;
    }
    scanline_instance * instance = scanlines_ + line_number;
    instance->line_number = line_number;
    instance->xscroll = xscroll;
    instance->yscroll = yscroll;
    instance->pading2 = 0;
    return 0;
}


// draw current frame
bool renderer::frame() {

    // init frame data
    begin();
    // copy instancing data to bgfx.     
    memcpy(idb_.data, scanlines_, sizeof(scanline_instance) * 240);
    // draw with instancing
    /* draw pattern tables */

    /* draw nametables (with palette) */


    /* draw the final background */
    {
        // set vertext buffer
        bgfx::setVertexBuffer(0, vtx_line_);
        bgfx::setIndexBuffer(idx_line_);
        // Set instance data buffer.
        bgfx::setInstanceDataBuffer(&idb_);

        // Set render states.
        bgfx::setState(0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_DEPTH_TEST_ALWAYS
        );

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, program_scanline_);
    }
    /* draw sprits */


    /* show debug infos */

    frames_++;
    bgfx::dbgTextClear();
    int64_t now = bx::getHPCounter();
    static int64_t last = now;
    const int64_t frameTime = now - last;
    last = now;
    const double freq = double(bx::getHPFrequency() );
    const double toMs = 1000.0 / freq;

    bgfx::dbgTextPrintf(90, 0, 0x4F, "fps = %7.3fms", toMs);
    bgfx::dbgTextPrintf(90, 1, 0x4F, "width = %d, height = %d", width_, height_);
    bgfx::dbgTextPrintf(90, 2, 0x4F, "frame: %ul", frames_);

    return true;
}


void renderer::begin() {
    bgfx::allocInstanceDataBuffer(&idb_, 240, sizeof(scanline_instance));
}