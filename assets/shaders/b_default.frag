#version 430

out vec4 _fragColor;

void main()
{
	vec3 diffuse_color = vec3(0.25, 0.73, 0.53);
	_fragColor = vec4(diffuse_color, 1.0);
}