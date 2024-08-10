#version 460 core

// Uniform buffers
uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjMatrix;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

out vec3 fragColor;

void main()
{
    vec4 position = vec4(inPosition, 1.0);
    position = WorldMatrix * position;
    position = ViewMatrix * position;
    position = ProjMatrix * position;

    gl_Position = position;
    fragColor = inColor;
}
