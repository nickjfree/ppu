$input v_scroll

#include <bgfx_shader.sh>

USAMPLER2D(s_color, 0);
SAMPLER2D(s_palette, 1);


uniform vec4 u_palette[2];


// vec4 fetch_pattern_color(uint pattern_id, int palette_index, ivec2 xy) {
//     ivec2 uv_low = ivec2(xy.y, pattern_id);
//     ivec2 uv_high = ivec2(xy.y + 8, pattern_id);
//     int low = texelFetch(s_pattern, uv_low, 0).x;
//     int high = texelFetch(s_pattern, uv_high, 0).x;
//     int shift = 7 - xy.x;
//     int color_id = ((low >> shift) & 0x01) +  (((high >> shift) & 0x01) << 1);
//     // fetch real color
//     shift = color_id << 3;
//     int color_index = int(floatBitsToInt(u_palette[0][palette_index]) >> shift) & 0x00ff; 
//     // fetch color
//     vec4 color = texelFetch(s_palette, ivec2(color_index, 0), 0);
//     return color;
// }

void main()
{
    float h = floor(v_scroll.x * 256);
    float v = floor(v_scroll.y * 240);


    uint color_bits = texelFetch(s_color, ivec2(h, v), 0).x;

    int palette_index = (int(color_bits) >> 2) & 0x0003; 
    int color_index = (int(color_bits)) & 0x0003;
    int shift = color_index << 3;
    int color_number = int(floatBitsToInt(u_palette[0][palette_index]) >> shift) & 0x00ff; 
    vec4 color = texelFetch(s_palette, ivec2(color_number, 0), 0);

    vec4 palette_color = vec4(palette_index/3.0, palette_index/3.0, palette_index/3.0, palette_index/3.0);

    vec4 color_color = vec4(color_index/3.0, color_index/3.0, color_index/3.0, 0);

    //color = palette_color * 0.5 + color_color * 0.5;
    gl_FragColor = color;
}
