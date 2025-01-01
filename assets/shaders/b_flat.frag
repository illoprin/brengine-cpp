#version 430

in vec2 out_uv;

// Entity
uniform bool u_use_texturing;
uniform sampler2D u_texture;
uniform vec4 u_color;

out vec4 _fragColor;

void main()
{
	vec4 diffuse_color;

	if (!u_use_texturing) diffuse_color = u_color;
	else if (u_use_texturing)
	{
		diffuse_color = texture(u_texture, out_uv);
		diffuse_color *= u_color;
		// Render empty if texture alpha is less then .05 value
		if (diffuse_color.a <= 0.05) discard;
	}

	_fragColor = diffuse_color;
}