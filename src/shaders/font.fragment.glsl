#version 330 core 

uniform sampler2D u_image;
uniform vec3 u_color;

in vec2 fg_uv;

out vec4 color;

void main() 
{
	vec4 alpha = vec4(1.0, 1.0, 1.0, texture(u_image, fg_uv).r);
	color = vec4(u_color, 1.0) * alpha;
	// color = vec4(1.0f, 1.0f, 1.0f, 1.0);
}