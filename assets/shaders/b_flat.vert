#version 430

in vec2 in_position;
in vec2 in_texcoord;

uniform mat4 u_model;
uniform mat4 u_projection;

out vec2 out_uv;

void main()
{
	out_uv = in_texcoord;
	gl_Position = u_projection * u_model * vec4(in_position, 0.0, 1.0);
}