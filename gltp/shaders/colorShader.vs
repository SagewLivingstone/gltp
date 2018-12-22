#version 400 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
out vec3 color;
out vec3 vertex_pos;

void main()
{
	color = vertex_color;
	vertex_pos = vertex_position;
	gl_Position = vec4(vertex_position, 1.f);
}