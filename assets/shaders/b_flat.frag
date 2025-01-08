#version 430

in vec2 out_uv;

// GUI Item
uniform bool u_use_texturing;
uniform bool u_use_transperency_mask = false;
uniform sampler2D u_texture;

uniform bool u_is_text = false;
uniform vec4 u_color = vec4(1.0);

uniform vec3 u_transperent_pixel;

out vec4 _fragColor;

vec4 DeleteTransperentAreas(vec4 src, vec3 mask)
{
	vec3 delta = abs(src.rgb - mask);
	float factor = length(delta);
	if (factor < 0.023) src.a = 0.0;
	return src;
}

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
			// Delete mask color if texture use transperency mask
			if (u_use_transperency_mask)
				diffuse_color = DeleteTransperentAreas(diffuse_color, u_transperent_pixel);
			
			diffuse_color *= u_color;
			// Render empty if texture alpha is less then .05 value
			if (diffuse_color.a <= 0.05) discard;
			_fragColor = diffuse_color;
		}

	}

}