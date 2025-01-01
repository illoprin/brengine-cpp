#version 430

in vec2 out_uv;
in vec3 out_normal;
in vec3 out_fragcoord;

// Entity
uniform bool u_use_texturing;
uniform sampler2D u_texture;
uniform vec4 u_color;

// Camera
uniform vec3 u_camera_position;

// Lighting
uniform vec3 u_ambient_light = vec3(0.96, 0.97, 1.0);

// Debug
uniform bool u_depth = false;

out vec4 _fragColor;

void main()
{
	vec4 diffuse_color;

	// Texturing
	if (!u_use_texturing)
		diffuse_color = u_color;
	else if (u_use_texturing)
	{
		diffuse_color = texture(u_texture, out_uv);
		diffuse_color *= u_color;
		// Render empty if texture alpha is less then .05 value
		if (diffuse_color.a <= 0.05) discard;
	}

	// Lighting
	float factor = dot(
		normalize(u_camera_position - out_fragcoord), 
		out_normal
	) * .3;
	vec3 lighting = u_ambient_light * (.7 + factor);

	// Final color
	if (!u_depth)
		_fragColor = diffuse_color * vec4(lighting, 1.0); // Textured
	else _fragColor = vec4(vec3(gl_FragCoord.w), 1.0); // Depth
}