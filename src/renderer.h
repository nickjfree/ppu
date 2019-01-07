#ifndef __RENDERER__
#define __RENDERER__

#include <memory.h>
#include "bgfx/bgfx.h" 
#include "common/common.h"
#include "shaders/vs_scanline.h"
#include "shaders/ps_scanline.h"
#include <vector>
#include <iostream>


using namespace std;


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
*  pattern table, will render to a 256 * 128 texture, two 128 * 128 textures
*/
struct pattern_instance 
{
    //  16 bytes
    float data[4];
};

/*
*   name table instance will render to a 512 * 512 texture
*/
struct name_instance
{
    float pattern_id;
    float r,g,b;
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
    std::vector<pattern_instance> patterns_;
    // pattern table cpu-buffer
    std::vector<name_instance> names_;

    // programes
    bgfx::VertexBufferHandle  vtx_line_;
    bgfx::IndexBufferHandle  idx_line_;
    bgfx::ProgramHandle program_scanline_;
    bgfx::ProgramHandle program_pattern_;
    bgfx::ProgramHandle program_nametable_;
    // conters
    int64_t frames_;
    int line_drawed_;

private:
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
    int scanline(float line_number, float xscroll, float yscroll);
    // flush all the buffered scanlines
    int flush();    
    // draw current frame
    bool frame();
};







#endif