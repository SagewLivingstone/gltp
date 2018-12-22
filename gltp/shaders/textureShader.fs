#version 400 core
in vec3 out_color;
in vec2 out_tex_coord;

out vec4 fragment_color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	fragment_color = mix(texture(texture1, out_tex_coord), texture(texture2, out_tex_coord), 0.2);
}