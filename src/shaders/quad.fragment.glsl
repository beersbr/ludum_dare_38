#version 330 core

uniform sampler2D image;

in vec3 fg_color;
in vec2 fg_uv;
in int fg_index;

out vec4 color;

void main(void) {
    
    float d = 1.0/4.0;
    vec2 uv = (fg_uv/d) + mod(fg_uv, d);
    color = texture(image, uv) * vec4(fg_color, 1.0);
}

