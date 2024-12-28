#version 430

in vec3 in_position;
in vec2 in_texcoord;
in vec3 in_normal;

void main()
{
	gl_Position = vec4(in_position, 1.0);
}