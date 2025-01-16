#version 430

in vec2 out_uv;

// Standart inputs
uniform vec2 u_window_size = vec2(1280, 720);
uniform float u_time = 1.0;

uniform sampler2D u_scene_view;

float find_closest(int x, int y, float c0) {
	/* 8x8 Bayer ordered dithering */
	int dither[8][8] = {
		{0, 32, 8, 40, 2, 34, 10, 42 },
		{48, 16, 56, 24, 50, 18, 58, 26 },
		{12, 44, 4, 36, 14, 46, 6, 38 },
		{60, 28, 52, 20, 62, 30, 54, 22 },
		{3, 35, 11, 43, 1, 33, 9, 41 },
		{51, 19, 59, 27, 49, 17, 57, 25 },
		{15, 47, 7, 39, 13, 45, 5, 37 },
		{63, 31, 55, 23, 61, 29, 53, 21 }
	};

	float limit = 0.0;
	if(x < 8) {
		limit = (dither[x][y] + 1) / 64.0;
	}

	if(c0 < limit) return 0.0;
	return 1.0;
}


out vec4 _fragColor;
void main()
{
	float scale = 3.0 + mod(2.0 * u_time, 32.0);

	vec2 pixel_pos = out_uv * u_window_size * scale;
	int mod_x = mod(floor(pixel_pos.x), 8);
	int mod_y = mod(floor(pixel_pos.y), 8);

	vec4 diffuse_color = texture(u_scene_view, out_uv);

	vec3 rgb = diffuse_color.rgb;

	rgb.r = find_closest(mod_x, mod_y, rgb.r);
	rgb.g = find_closest(mod_x, mod_y, rgb.g);
	rgb.b = find_closest(mod_x, mod_y, rgb.b);

	_fragColor = diffuse_color;
}