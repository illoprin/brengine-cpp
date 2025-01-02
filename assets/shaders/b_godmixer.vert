#version 430

in vec2 in_position;
in vec2 in_texcoord;

out vec2 out_uv;

void main()
{
	out_uv = in_texcoord;
	gl_Position = vec4(in_position, 0.0,  1.0);
}