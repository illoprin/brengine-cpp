#version 430

in vec3 in_position;
in vec2 in_texcoord;
in vec3 in_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform float u_uv_scaling;

out vec2 out_uv;
out vec3 out_normal;
out vec3 out_fragcoord;

void main()
{
	// Calculate normal
	out_normal = mat3(transpose(inverse(u_model))) * in_normal;
	// Calculate fragment position
	out_fragcoord = vec3((u_model * vec4(in_position, 1.0)).xyz);
	// Send texcoord info
	out_uv = in_texcoord * u_uv_scaling;


	// Get final vertex position
	gl_Position = u_projection * u_view * u_model * vec4(in_position, 1.0);
}