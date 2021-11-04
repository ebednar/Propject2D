#version 410 core

layout(location = 0) out vec4	FragColor;
layout(location = 2) out int	id;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
	vec4 col = texture(screenTexture, TexCoords).rgba;
	FragColor = col;
	id = 50;
}