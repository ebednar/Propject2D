#version 410 core

in vec2 vTexCoord;

uniform sampler2D diffuse;

layout(location = 0) out vec4	fColor;

void main()
{
	vec4 texColor = texture(diffuse, vTexCoord).rgba;
	

	fColor = vec4(texColor);
}