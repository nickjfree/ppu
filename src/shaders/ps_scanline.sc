$input v_scroll

#include <bgfx_shader.sh>


void main()
{
    float h = floor(v_scroll.x * 256);
    float v = floor(v_scroll.y * 240);

    float th = floor(v_scroll.x * 16);
    float tv = floor(v_scroll.y * 15);

    gl_FragColor = vec4(th/16, tv/16, 0, 0);
}