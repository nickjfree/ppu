$input a_position, i_data0
$output v_scroll

#include <bgfx_shader.sh>

#define d_width   (1/256.0f)
#define d_height  (1/240.0f)


void main()
{
    // uint line_start = i_data0.z;
    // uint line_end = i_data0.w
    float start_y = i_data0.z * d_height;
    float end_y = (i_data0.w+1) * d_height;
    float clip_y =  mix(start_y, end_y, (a_position.y * -0.5 + 0.5));
    clip_y = - clip_y * 2 + 1;
    gl_Position = vec4(a_position.x, clip_y, 0, 1);
    vec2  uv = gl_Position.xy * vec2(0.5, -0.5) + 0.5;
    v_scroll = vec4(uv, i_data0.xy);
}
