#version 430

in vec2 out_uv;

// Entity
uniform sampler2D u_texture;
uniform bool u_use_texturing;
uniform vec3 u_color;
uniform float u_alpha;

out vec4 _fragColor;

void main()
{
	vec4 diffuse_color;

	if (!u_use_texturing) diffuse_color = vec4(u_color, u_alpha);
	else if (u_use_texturing)
	{
		diffuse_color = texture(u_texture, out_uv);
		diffuse_color.a *= u_alpha;
		// Render empty if texture alpha is less then .05 value
		if (diffuse_color.a <= 0.05) discard;
	}

	_fragColor = diffuse_color;
}