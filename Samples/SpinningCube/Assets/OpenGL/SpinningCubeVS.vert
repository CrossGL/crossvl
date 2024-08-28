#version 460 core

layout(std140, binding = 0) uniform Matrices
{
    mat4 WorldMatrix;
    mat4 ViewMatrix;
    mat4 ProjMatrix;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

out vec3 fragColor;

void main()
{
    vec4 position = vec4(inPosition, 1.0);
    position = ProjMatrix * ViewMatrix * WorldMatrix * position;

    gl_Position = position;
    fragColor = inColor;
}