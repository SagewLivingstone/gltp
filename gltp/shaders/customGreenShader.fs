#version 400 core

in vec3 vertex_pos;

out vec4 fragment_color;

uniform vec4 customColor;

void main()
{
	fragment_color = vec4(vertex_pos, 1.f);
}