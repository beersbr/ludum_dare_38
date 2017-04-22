#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light1_direction;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 uv;

out vec3 fg_color;

void main(void) {
	fg_color = color;
	gl_Position = projection * view * model * vec4(position, 1.0);
}

