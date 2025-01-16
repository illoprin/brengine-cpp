#version 430

in vec3 out_normal;
in vec2 out_uv;
in vec3 out_fragcoord;
flat in int out_texture_id;
flat in int out_texture_type;
in float out_glow_intensity;

/*
TextureTypes:
	BL_TEXTURE_NULL = 0,
	BL_TEXTURE_COLOR = 1,
	BL_TEXTURE_FLAT = 2,
	BL_GLOWING = 3,
	BL_NUM_FLATTYPES = 4
*/

// Fog
vec3 fog_color = vec3(0.14, 0.16, 0.17);

// Lighting
uniform float u_gamma = 2.2;
uniform vec3 u_ambient_light = vec3(0.96, 0.97, 1.0);

// Texture
uniform sampler2DArray u_atlas;
uniform sampler2DArray u_palette;
uniform sampler2D u_texture_null;

// Camera
uniform vec3 u_camera_position = vec3(1.0);

uniform vec3 u_transperent_pixel;

// Number of layers in texture atlas
uniform int u_texture_layers = 1;
// Number of layers in palette atlas
uniform int u_palette_layers = 1;

// Debug
uniform bool u_color_only = false;
uniform bool u_depth = false;
uniform bool u_normals = false;

vec4 DeleteTransperentAreas(vec4 src, vec3 mask)
{
	vec3 delta = abs(src.rgb - mask);
	float factor = length(delta);
	if (factor < 0.023) src.a = 0.0;
	return src;
}

// Calculate Z coord in texcoord of texture storage
float getActualLayer(int num, int layer)
{
	return max(0.0, min(float(num - 1.0), floor(float(layer) + .5)) );
}

out vec4 _fragColor;

void main()
{
	const float inv_gamma = 1.0 / u_gamma;

	// Diffuse texture
	vec4 diffuse_color = vec4(1.0);
	
	// -- Select texture
	if (out_texture_type == 0)
		// Null texture
		diffuse_color = texture(u_texture_null, out_uv);
	else if (out_texture_type == 1 || out_texture_type == 3)
	{
		// Palette color
		diffuse_color = texture(
			u_palette, 
			vec3(out_uv, float(out_texture_id))
		);
	}
	else if (out_texture_type == 2)
	{
		// Flat texture
		diffuse_color = texture(
			u_atlas,
			vec3(out_uv, float(out_texture_id))
		);
		diffuse_color = 
			DeleteTransperentAreas(diffuse_color, u_transperent_pixel);
	}
	
	// Enter to changing gamma mode
	diffuse_color.rgb = pow(diffuse_color.rgb, vec3(u_gamma));

	// -- Light
	if (out_texture_type != 3)
	{
		float al_factor = dot(
			normalize(u_camera_position - out_fragcoord),
			out_normal
		);
		vec3 lighting = u_ambient_light * (0.5 + (al_factor * 0.5));
		// Apply lighting
		diffuse_color.rgb *= lighting;
	}
	else
	{
		// Apply glow
		diffuse_color *= out_glow_intensity;
	}
	
	// Return to linear gamma
	diffuse_color.rgb = pow(diffuse_color.rgb, vec3(inv_gamma));
	

	// -- Out fragcolor
	if (u_color_only)
		// Wireframe rendering
		_fragColor = vec4(1.0);
	else if (u_depth)
		// Depth
		_fragColor = vec4(vec3(gl_FragCoord.w), 1.0);
	else if (u_normals)
		// Render normals
		_fragColor = vec4(normalize(vec3(0.5) + (out_normal * 0.5)), 1.0);
	else
		_fragColor = diffuse_color;
}