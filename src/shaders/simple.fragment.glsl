#version 330 core

uniform sampler2D image;

in vec3 fg_color;
in vec2 fg_uv;

out vec4 color;

void main(void) {
    color = texture(image, fg_uv) * vec4(fg_color, 1.0);
}

