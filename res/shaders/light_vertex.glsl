#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normCoord;
layout(location = 3) in int id;

flat out int vId;

uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;

void main()
{
    vId = id;
    gl_Position = u_P * u_V * u_M * vec4(position, 1.0f);
}