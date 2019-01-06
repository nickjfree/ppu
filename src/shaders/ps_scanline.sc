$input v_scroll

#include <bgfx_shader.sh>

void main()
{
    gl_FragColor = vec4(v_scroll.z/240.0, 0, 0, 1);
}