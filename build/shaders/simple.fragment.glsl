#version 330 core

in vec3 fg_color;
out vec4 color;

void main(void) {
	color = vec4(fg_color, 1.0);
}

