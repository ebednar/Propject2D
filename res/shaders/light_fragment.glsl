#version 410 core

layout(location = 0) out vec4	color;
layout(location = 1) out int	color2;

struct Light {
	vec3 diffuse;
};

uniform Light light;

void main()
{
	color = vec4(light.diffuse, 1.0f);
	color2 = 50;
}