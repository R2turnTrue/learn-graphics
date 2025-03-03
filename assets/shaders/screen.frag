#version 330 core

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

#define MAX_LIGHTS 15

out vec4 FragColor;

in vec2 uv;

struct Light {
    // 0 - Directional Light
    // 1 - Point Light
    // 2 - Spot Light
    int lightType;

    // Directional Light
    vec3 direction;

    // Point Light
    vec3 position;
    float constant;
    float linear;
    float quadratic;

    // Spot Light
    float cutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;
uniform sampler2D gShininess;
uniform sampler2D gDepth;

uniform vec3 viewPos;
uniform Light lights[MAX_LIGHTS];
uniform int lightCounts;

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
    //FragColor = vec4(vec3(texture(gDepth, uv).r), 1.0);
    //return;
    vec4 output = vec4(0.0);

    vec3 fragPos = texture(gPosition, uv).xyz;
    vec3 norm = texture(gNormal, uv).rgb;
    vec4 gDiff = texture(gDiffuse, uv);
    vec4 gSpec = texture(gSpecular, uv);
    vec4 gShin = texture(gShininess, uv);
    float shininess = gShin.r;

    if (gShin.g > 0.5)
    {
        FragColor = gDiff;
        return;
    }

    for (int i = 0; i < lightCounts; i++)
    {
        Light light = lights[i];

        // AMBIENT
        vec4 ambient = vec4(light.ambient, 1.0) * gDiff;

        // LIGHT DIR
        vec3 lightDir = normalize(-light.direction);

        if (light.lightType == LIGHT_POINT || light.lightType == LIGHT_SPOT)
        {
            lightDir = normalize(light.position - fragPos);
        }

        // DIFFUSE
        float diff = max(dot(norm, lightDir), 0.0);
        vec4 diffuse = gDiff * diff * vec4(light.diffuse, 1.0);

        // SPECULAR
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec4 specular = gSpec * spec * vec4(light.specular, 1.0);

        if (light.lightType == LIGHT_POINT || light.lightType == LIGHT_SPOT)
        {
            float dist = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;
        }

        if (light.lightType == LIGHT_SPOT)
        {
            float theta = dot(lightDir, normalize(-light.direction));
            float epsilon   = light.cutoff - light.outerCutoff;
            float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

            diffuse *= intensity;
            specular *= intensity;
        }

        vec4 result = ambient + diffuse + specular;

        output += result;
    }

    FragColor = output;
}