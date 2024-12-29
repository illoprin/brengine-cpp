#version 430

in vec3 in_position;
in vec2 in_texcoord;
in vec3 in_normal;

uniform mat4 u_model;

out vec2 out_uv;

void main()
{
	out_uv = in_texcoord;
	gl_Position = u_model * vec4(in_position, 1.0);
}