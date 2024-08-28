#version 460 core

in vec3 vColor;
in vec3 vNormal;

out vec4 outColor;

void main()
{
    outColor = vec4(vNormal, 1.0);
}
