static const uint8_t ps_scanline[174] =
{
	0x46, 0x53, 0x48, 0x06, 0xd4, 0x98, 0x71, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9b, 0x00, // FSH...q.........
	0x00, 0x00, 0x69, 0x6e, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x76, 0x5f, 0x73, 0x63, 0x72, 0x6f, // ..in vec4 v_scro
	0x6c, 0x6c, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, // ll;.void main ()
	0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, // .{.  vec4 tmpvar
	0x5f, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x79, // _1;.  tmpvar_1.y
	0x7a, 0x77, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 0x33, 0x28, 0x30, 0x2e, 0x30, 0x2c, 0x20, 0x30, // zw = vec3(0.0, 0
	0x2e, 0x30, 0x2c, 0x20, 0x31, 0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, // .0, 1.0);.  tmpv
	0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x20, 0x3d, 0x20, 0x28, 0x76, 0x5f, 0x73, 0x63, 0x72, 0x6f, // ar_1.x = (v_scro
	0x6c, 0x6c, 0x2e, 0x7a, 0x20, 0x2f, 0x20, 0x32, 0x34, 0x30, 0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x20, // ll.z / 240.0);. 
	0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20, //  gl_FragColor = 
	0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,             // tmpvar_1;.}...
};