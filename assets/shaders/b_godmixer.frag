#version 430

in vec2 out_uv;

uniform sampler2D _scene;
uniform sampler2D _ui;

uniform float u_contrast = 1.04;
uniform float u_brightness = 1.0;

out vec4 _fragColor;
void main()
{
	vec4 scene_color = texture(_scene, out_uv);
	// -- Color correction
	scene_color.rgb = ((scene_color.rgb - .5) * max(u_contrast, 0.0)) + .5; // Apply contrast
	scene_color.rgb *= u_brightness; // Apply brightness

	vec4 ui_color = texture(_ui, out_uv);
	vec4 color = mix(scene_color, ui_color, ui_color.a);
	_fragColor = color;
}