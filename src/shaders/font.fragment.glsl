#version 330 core 

uniform sampler2D u_image;
uniform vec3 u_color;

in vec2 fg_uv;

out color;

void main() 
{
	vec3 alpha = vec4(1.0, 1.0, 1.0, texture(u_iamge, fg_uv).r);
	color = vec4(color, 1.0) * alpha;
}