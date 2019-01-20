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
void renderer::init(const vram * memory) {

    memory_ = memory; 

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

    // create palette texture
    tex_palette_ = bgfx::createTexture2D(64, 1, false, 1,
        bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_NONE|BGFX_SAMPLER_POINT, nullptr);

    // create fame texture
    tex_frame_ = bgfx::createTexture2D(256, 240, false, 1, 
        bgfx::TextureFormat::R8U, BGFX_TEXTURE_NONE|BGFX_SAMPLER_POINT, nullptr);

    // update data to palette texture
    bgfx::updateTexture2D(tex_palette_, 0, 0, 0, 0, 64, 1, bgfx::makeRef(palette_data, 256));

    // create uniforms
    s_color_ = bgfx::createUniform("s_color", bgfx::UniformType::Int1, 1);
    // create uniforms for 
    s_palette_ = bgfx::createUniform("s_palette", bgfx::UniformType::Int1, 1);
    // create palette, 2 float4 type
    u_palette_ = bgfx::createUniform("u_palette", bgfx::UniformType::Vec4, 2);
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


int renderer::scanline(float line_number, float xscroll, float yscroll) {
    // fill scanline cpu buffer
    if (line_number < 0 || line_number > 239) {
        return -1;
    }
    if (scanlines_.empty()) {
        scanline_instance instance{
            xscroll,
            yscroll,
            line_number,
            line_number
        };
        scanlines_.push_back(instance);
    } else {
        scanline_instance& prev = scanlines_.back();  
        if (xscroll == prev.xscroll && yscroll == prev.yscroll) {
            // status is not changed
            prev.line_end = line_number;
        } else {
            scanline_instance instance{
                xscroll,
                yscroll,
                line_number,
                line_number
            };
            scanlines_.push_back(instance);
        }
    }
    return 0;
}

int renderer::submit(void * data, size_t size) {
    // update color buffer
    bgfx::updateTexture2D(tex_frame_, 0, 0, 0, 0, 256, 240, bgfx::makeRef(data, size));

    bgfx::setUniform(u_palette_, memory_->palette());
    // set vertext buffer
    bgfx::setVertexBuffer(0, vtx_line_);
    bgfx::setIndexBuffer(idx_line_);

    // Set render states.
    bgfx::setState(0
        | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_DEPTH_TEST_ALWAYS
    );
    // set color texture
    bgfx::setTexture(0, s_color_, tex_frame_);
    // set palette texture
    bgfx::setTexture(1, s_palette_, tex_palette_);
    // Submit primitive for rendering to view 0.
    bgfx::submit(0, program_scanline_);
    return 0;
}

// draw infos about current frame
bool renderer::frame() {

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
    bgfx::dbgTextPrintf(90, 2, 0x4F, "frame: %d", frames_);
    bgfx::dbgTextPrintf(90, 1, 0x4F, "lines: %d", line_drawed_);

    // clear counter
    line_drawed_ = 0;
    return true;
}
