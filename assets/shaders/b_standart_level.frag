#version 430

in vec3 out_normal;
in vec2 out_uv;
in vec3 out_fragcoord;
in vec3 out_color;
flat in int out_texture_id;
flat in int out_texture_type;

uniform sampler2DArray u_texure;
uniform vec3 u_camera_position = vec3(1.0);

out vec4 _fragColor;

void main()
{
	// Diffuse texture
	vec4 diffuse_color;

	// Light
	vec3 lighting;

	// Fog

	// Out fragcolor
	_fragColor = diffuse_color * vec4(lighting,1.0);
}