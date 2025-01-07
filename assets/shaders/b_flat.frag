#version 430

in vec2 out_uv;

// GUI Item
uniform bool u_use_texturing;
uniform bool u_is_text = false;
uniform sampler2D u_texture;
uniform vec4 u_color = vec4(1.0);

out vec4 _fragColor;

void main()
{
	if (!u_use_texturing)
	{
		_fragColor = u_color;
	}
	else if (u_use_texturing)
	{
		vec4 diffuse_color = texture(u_texture, out_uv);
		// Text rendering
		if (u_is_text)
		{
			vec4 sampled = vec4(u_color.rgb, diffuse_color.r);
			_fragColor = sampled * u_color.a;
		}
		// Other types rendering
		else
		{
			diffuse_color *= u_color;
			// Render empty if texture alpha is less then .05 value
			if (diffuse_color.a <= 0.05 && !u_is_text) discard;
			_fragColor = diffuse_color;
		}

	}

}