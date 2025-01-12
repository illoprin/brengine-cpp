#version 430

layout (location = 0) in vec3 in_position;
layout (location = 2) in vec2 in_texcoord;
layout (location = 3) in vec3 in_normal;
layout (location = 4) in vec3 in_color;
layout (location = 5) in int in_texture_layer;
layout (location = 6) in int in_texture_type;

out vec3 out_normal;
out vec2 out_uv;
out vec3 out_fragcoord;
out vec3 out_color;
flat out int out_texture_layer;
flat out int out_texture_type;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
	out_normal = normalize(in_normal);
	out_uv = in_texcoord;
	out_color = in_color;
	out_fragcoord = in_position;
	out_texture_layer = in_texture_layer;
	out_texture_type = in_texture_type;

	gl_Position = u_projection * u_view * vec4(in_position, 1.0);
}