#version 410 core

out vec4 color;

struct Light {
	vec3 diffuse;
};

uniform Light light;

void main()
{
	color = vec4(light.diffuse, 1.0f);
}