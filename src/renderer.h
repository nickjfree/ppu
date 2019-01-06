#ifndef __RENDERER__
#define __RENDERER__

#include <memory.h>
#include "bgfx/bgfx.h" 
#include "common/common.h"
#include "shaders/vs_scanline.h"
#include "shaders/ps_scanline.h"

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
*  scanline instance data
*/

struct scanline_instance
{
    float xscroll;
    float yscroll;
    float line_number;
    float pading2;
};




class renderer 
{

private:
    // frame buffer size
    uint32_t width_;
    uint32_t height_;
    uint32_t debug_;
    uint32_t reset_;
    // scanline instancing buffer
    bgfx::InstanceDataBuffer idb_;
    // scanline instance cpu-buffer
    scanline_instance scanlines_[240];
    // programes
    bgfx::VertexBufferHandle  vtx_line_;
    bgfx::IndexBufferHandle  idx_line_;
    bgfx::ProgramHandle program_scanline;
    bgfx::ProgramHandle program_pattern;
    bgfx::ProgramHandle program_nametable;
    // conters
    int64_t frames_;


private:
    // begin next frame
    void begin();
    // create progeram
    bgfx::ProgramHandle CreateProgram(const uint8_t * vs_code, uint32_t vs_size, const uint8_t * ps_code, uint32_t ps_size);
 
public:
    renderer();
    // init
    void init();
    // shutdown
    void shutdown();
    // update
    bool update();
    // submit a scanline drawcall
    int scanline(int line_number, float xscroll, float yscroll);
    // draw current frame
    bool frame();
};







#endif