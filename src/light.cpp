#include "light.h"


DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {

        this->lightType = LIGHT_DIRECTIONAL;
        this->lightDirection = direction;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;

}

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
    float constant, float linear, float quadratic) {

        this->lightType = LIGHT_POINT;
        this->lightPos = position;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;

}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
    float cutoffInDegrees, float outerCutoffInDegrees,
    float constant, float linear, float quadratic) {

        this->lightType = LIGHT_SPOT;
        this->lightPos = position;
        this->lightDirection = direction;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->cutoffDegree = cutoffInDegrees;
        this->outerCutoffDegree = outerCutoffInDegrees;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;

}