#ifndef LIGHT_H
#define LIGHT_H

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
public:
    int lightType;

    glm::vec3 lightPos;
    glm::vec3 lightDirection;
    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float cutoffDegree;
    float outerCutoffDegree;
};

class DirectionalLight : public Light
{
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};

class PointLight : public Light
{
public:
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);
};

class SpotLight : public Light
{
public:
    SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
        float cutoffInDegrees = 12.5f, float outerCutoffInDegrees = 20.0f,
        float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);
};

#endif