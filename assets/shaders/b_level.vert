#version 430

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texcoord;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in int in_texture_layer;
layout (location = 4) in int in_texture_type;
layout (location = 5) in float in_glow_intensity;

out vec3 out_normal;
out vec2 out_uv;
out vec3 out_fragcoord;
flat out int out_texture_id;
flat out int out_texture_type;
out float out_glow_intensity;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
	out_normal = normalize(in_normal);
	out_uv = in_texcoord;
	out_fragcoord = in_position;
	out_texture_id = in_texture_layer;
	out_texture_type = in_texture_type;
	out_glow_intensity = in_glow_intensity;

	gl_Position = u_projection * u_view * vec4(in_position, 1.0);
}