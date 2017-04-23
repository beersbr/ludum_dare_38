#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light1_position;
uniform vec3 light1_direction;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 uv;

out vec3 fg_color;

void main(void) {
	vec3 offset_normal = vec3(model * vec4(normal, 1.0));
	vec3 light_offset  = normalize(light1_position - vec3(model * vec4(position, 1.0)));
	float light        = dot(normal, light_offset);

	fg_color = color * light;
	gl_Position = projection * view * model * vec4(position, 1.0);
}

