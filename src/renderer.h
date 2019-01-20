#ifndef __RENDERER__
#define __RENDERER__

#include <memory.h>
#include "bgfx/bgfx.h" 
#include "common/common.h"
#include "shaders/vs_scanline.h"
#include "shaders/ps_scanline.h"
#include <vector>
#include <iostream>

#include "vram.h"


using namespace std;

/*
*   palette data
*/
static const uint8_t palette_data[256] = {
 84,  84,  84,  0,     0,  30, 116, 0,    8,  16, 144, 0,     48,   0, 136, 0,    68,   0, 100, 0,    92,   0,  48, 0,    84,   4,   0, 0,    60,  24,   0, 0,     32,  42,   0, 0,      8,  58,   0, 0,     0,  64,   0,  0,    0,  60,   0, 0,     0,  50,  60, 0,    0,   0,   0, 0,   0, 0, 0, 0,  0, 0, 0, 0,  
152, 150, 152,  0,     8,  76, 196, 0,   48,  50, 236, 0,     92,  30, 228, 0,   136,  20, 176, 0,   160,  20, 100, 0,   152,  34,  32, 0,   120,  60,   0, 0,     84,  90,   0, 0,     40, 114,   0, 0,     8, 124,   0,  0,    0, 118,  40, 0,     0, 102, 120, 0,    0,   0,   0, 0,   0, 0, 0, 0,  0, 0, 0, 0,  
236, 238, 236,  0,    76, 154, 236, 0,  120, 124, 236, 0,    176,  98, 236, 0,   228,  84, 236, 0,   236,  88, 180, 0,   236, 106, 100, 0,   212, 136,  32, 0,    160, 170,   0, 0,    116, 196,   0, 0,    76, 208,  32,  0,   56, 204, 108, 0,    56, 180, 204, 0,   60,  60,  60, 0,   0, 0, 0, 0,  0, 0, 0, 0,  
236, 238, 236,  0,   168, 204, 236, 0,  188, 188, 236, 0,    212, 178, 236, 0,   236, 174, 236, 0,   236, 174, 212, 0,   236, 180, 176, 0,   228, 196, 144, 0,    204, 210, 120, 0,    180, 222, 120, 0,   168, 226, 144,  0,  152, 226, 180, 0,   160, 214, 228, 0,  160, 162, 160, 0,   0, 0, 0, 0,  0, 0, 0, 0,  
};


/*
*   vertex buffer define
*/
struct quad_vertex
{
    float x, y, z;
    static void init()
    {
        vtx_decl
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();
    };
    static bgfx::VertexDecl vtx_decl;
};


/*
*  scanline instance data, a batch of line.  share the same pattern name tables.
*/

struct scanline_instance
{
    float xscroll;
    float yscroll;
    float line_start;
    float line_end;
};

/*
*  pattern table,  a 256 * 128 texture, two 128 * 128 textures
*/
struct pattern_data 
{
    float x;
    float y;
    float color;
};

/*
*   name table data 4 * 32 * 30 texture
*/
struct name_data
{
    float x;
    float y;
    float attribute;
};



class renderer 
{

private:
    // frame buffer size
    uint32_t width_;
    uint32_t height_;
    uint32_t debug_;
    uint32_t reset_;
    // scanline instance cpu-buffer
    std::vector<scanline_instance> scanlines_;
    // pattern table cpu-buffer
    std::vector<pattern_data> patterns_;
    // pattern table cpu-buffer
    std::vector<name_data> names_;

    // programes
    bgfx::VertexBufferHandle  vtx_line_;
    bgfx::IndexBufferHandle  idx_line_;
    bgfx::ProgramHandle program_scanline_;
    // textures
    bgfx::TextureHandle tex_pattern_;
    bgfx::TextureHandle tex_nametable_;
    bgfx::TextureHandle tex_palette_;
    bgfx::TextureHandle tex_frame_;
    // uniforms
    bgfx::UniformHandle s_color_;
    bgfx::UniformHandle s_palette_;
    // palette constent buffer
    bgfx::UniformHandle u_palette_;


    // conters
    int64_t frames_;
    int line_drawed_;

    // vram data
    const vram * memory_;

private:
    // create progeram
    bgfx::ProgramHandle CreateProgram(const uint8_t * vs_code, uint32_t vs_size, const uint8_t * ps_code, uint32_t ps_size);
 
public:
    renderer();
    // init
    void init(const vram * memory);
    // shutdown
    void shutdown();
    // update
    bool update();
    // submit a scanline drawcall
    int scanline(float line_number, float xscroll, float yscroll); 
    // submit pixel color id
    int submit(void * data, size_t size);
    // flush all the buffered scanlines
    int flush();    
    // draw current frame
    bool frame();
};







#endif