$input a_position
$output v_scroll

#include <bgfx_shader.sh>

#define d_width   (1/256.0f)
#define d_height  (1/240.0f)


void main()
{
    gl_Position = vec4(a_position.x, a_position.y, 0, 1);
    vec2  uv = gl_Position.xy * vec2(0.5, -0.5) + 0.5;
    v_scroll = vec4(uv, 0, 0);
}