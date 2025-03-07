#version 330 core

// gPosition, gNormal, gDiffuse, gSpecular, gShininess
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec4 gSpecular;
layout (location = 4) out vec4 gShininess;
layout (location = 5) out vec4 gLightPosition;

struct Material {
    sampler2D diffuse0;
    sampler2D specular0;

    vec4 diffuse_col0;
    bool diffuse_col0_for_col;
    vec4 specular_col0;
    bool specular_col0_for_col;

    float shininess;
};

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec4 fragPosLightSpace;

uniform vec3 viewPos;
uniform Material material;

void main()
{
    gPosition = fragPos;
    gNormal = normalize(normal);
    gLightPosition = fragPosLightSpace;
    
    vec4 dif0 = texture(material.diffuse0, texCoord);
    vec4 spec0 = texture(material.specular0, texCoord);
    
    if (material.diffuse_col0_for_col)
        dif0 = material.diffuse_col0;
    
    if (material.specular_col0_for_col)
        spec0 = material.specular_col0;

    gDiffuse = dif0;
    gSpecular = spec0;
    gShininess = vec4(material.shininess, 0.0, 0.0, 1.0);
}