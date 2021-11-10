#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normCoord;
layout(location = 3) in int id;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos;
flat out int vId;

uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;
//uniform mat4 u_MVP;

void main()
{
    vFragPos = vec3(u_M * vec4(position, 1.0f));
    vTexCoord = texCoord;
    vNormal = mat3(transpose(inverse(u_M))) * normCoord;
    vId = id;

    gl_Position = u_P * u_V * u_M * vec4(position, 1.0f);
   /* gl_Position = u_MVP * vec4(position, 1.0f);*/
}