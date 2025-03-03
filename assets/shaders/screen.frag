#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexture;

vec4 kawaseBlur(vec2 uv, sampler2D originalTexture, float pixelOffset, vec2 texelSize)
{
    vec4 o = vec4(0.0);
    o += texture(originalTexture, uv + (vec2(pixelOffset + 0.5, pixelOffset + 0.5) * texelSize)) * 0.125;
    o += texture(originalTexture, uv + (vec2(-pixelOffset - 0.5, pixelOffset + 0.5) * texelSize)) * 0.125;
    o += texture(originalTexture, uv + (vec2(-pixelOffset - 0.5, -pixelOffset - 0.5) * texelSize)) * 0.125;
    o += texture(originalTexture, uv + (vec2(pixelOffset + 0.5, -pixelOffset - 0.5) * texelSize)) * 0.125;
    o += texture(originalTexture, uv + (vec2(pixelOffset + 0.5, 0.0) * texelSize)) * 0.125;
    o += texture(originalTexture, uv + (vec2(-pixelOffset - 0.5, 0.0) * texelSize)) * 0.125;
    o += texture(originalTexture, uv + (vec2(0.0, -pixelOffset - 0.5) * texelSize)) * 0.125;
    o += texture(originalTexture, uv + (vec2(0.0, pixelOffset + 0.5) * texelSize)) * 0.125;

    return o;
}

void main()
{
    //vec3 col = texture(screenTexture, uv).rgb;
    float blur = 2;
    vec2 texelSize = vec2(0.003, 0.003);

    vec3 col = kawaseBlur(uv, screenTexture, blur, texelSize).rgb;
    FragColor = vec4(col, 1.0);
}