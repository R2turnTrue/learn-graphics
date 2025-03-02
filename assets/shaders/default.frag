#version 330 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 texCoord;

uniform vec4 ourColor;
uniform float mixFactor;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
    FragColor = mix(texture(ourTexture, texCoord), texture(ourTexture2, texCoord), mixFactor) * vertexColor;
}