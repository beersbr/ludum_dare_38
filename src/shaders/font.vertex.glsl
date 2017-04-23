#version 330 core 

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 fg_uv;

void main() 
{
	fg_uv = uv;
	gl_Position = projection * view * model * vec4(font_vertex.xy, 0.0, 1.0);
}