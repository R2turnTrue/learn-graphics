#version 330 core

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

#define MAX_LIGHTS 15

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

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

out vec4 FragColor;
in vec4 vertexColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec3 viewPos;
uniform Material material;
uniform Light lights[MAX_LIGHTS];
uniform int lightCounts;

void main()
{
    //vec4 objColor = mix(texture(ourTexture, texCoord), texture(ourTexture2, texCoord), mixFactor);

    vec4 output = vec4(0.0);

    for (int i = 0; i < lightCounts; i++)
    {
        Light light = lights[i];

        //// AMBIENT
        vec4 ambient = vec4(light.ambient, 1.0) * texture(material.diffuse, texCoord);

        //// NORMAL & LIGHT DIRECTION
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(-light.direction);
        
        if (light.lightType == LIGHT_POINT || light.lightType == LIGHT_SPOT)
        {
            lightDir = normalize(light.position - fragPos);
        }

        //// DIFFUSE
        float diff = max(dot(norm, lightDir), 0.0);
        vec4 diffuse = texture(material.diffuse, texCoord) * diff * vec4(light.diffuse, 1.0);

        //// SPECULAR
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec4 specular = texture(material.specular, texCoord) * spec * vec4(light.specular, 1.0);

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