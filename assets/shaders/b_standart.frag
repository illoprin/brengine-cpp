#version 430

in vec2 out_uv;
in vec3 out_normal;
in vec3 out_fragcoord;

// Entity
// Texture options
uniform bool u_use_texturing;
uniform sampler2D u_texture;
uniform bool u_use_transperency_mask = false;

uniform vec4 u_color;

// Camera
uniform vec3 u_camera_position;

// Lighting
uniform float u_gamma = 2.2;
uniform vec3 u_ambient_light = vec3(0.96, 0.97, 1.0);

// Fog
vec3 fog_color = vec3(0.14, 0.16, 0.17);

// Debug
uniform bool u_depth = false;
uniform bool u_rnormal = false;

uniform vec3 u_transperent_pixel;

vec4 DeleteTransperentAreas(vec4 src, vec3 mask)
{
	vec3 delta = abs(src.rgb - mask);
	float factor = length(delta);
	if (factor < 0.023) src.a = 0.0;
	return src;
}

out vec4 _fragColor;

void main()
{
	const float inv_gamma = 1 / u_gamma;
	vec4 diffuse_color;

	// Texturing
	if (!u_use_texturing)
		diffuse_color = u_color;
	else if (u_use_texturing)
	{
		diffuse_color = texture(u_texture, out_uv);
		diffuse_color *= u_color;
		
		// Delete mask color if texture use transperency mask
		if (u_use_transperency_mask)
			diffuse_color =
				DeleteTransperentAreas(diffuse_color, u_transperent_pixel);
		
		// Render empty if texture alpha is less then .05 value
		if (diffuse_color.a <= .05) discard;
	}

	// Enter gamma to changing mode
	diffuse_color.rgb = pow(diffuse_color.rgb, vec3(u_gamma));

	// Lighting
	float factor = dot(
		normalize(u_camera_position - out_fragcoord), 
		out_normal
	) * .3;
	vec3 lighting = u_ambient_light * (.7 + factor);
	diffuse_color.rgb *= lighting;

	// Fog
	// exp - e^x
    float fog_dist = gl_FragCoord.w;
	float fog_factor = max(1.0 - exp(fog_dist - .2), 0.0);
	diffuse_color.rgb = mix(diffuse_color.rgb, fog_color, fog_factor);

	// Return to standart gamma
	diffuse_color.rgb = pow(diffuse_color.rgb, vec3(inv_gamma));


	// Final color
	if (u_depth)
		_fragColor = vec4(vec3(gl_FragCoord.w), 1.0); // Depth
	else if (u_rnormal)
		_fragColor = vec4(normalize(vec3(1.0) + out_normal), 1.0);
	else
		_fragColor = diffuse_color; // Textured
}