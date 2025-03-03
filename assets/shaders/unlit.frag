#version 330 core

layout (location = 2) out vec3 gDiffuse;
layout (location = 4) out vec4 gShininess;

uniform vec4 ourColor;

void main()
{
    gDiffuse = ourColor.rgb;
    gShininess = vec4(0.0, 1.0, 0.0, 0.0);
}