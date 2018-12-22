#version 400 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 tex_coord;

out vec3 out_color;
out vec2 out_tex_coord;

void main()
{
	gl_Position = vec4(vertex_position, 1.f);
	out_color = vertex_color;
	out_tex_coord = tex_coord;
}