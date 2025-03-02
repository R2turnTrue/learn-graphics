# define M_PI 3.14159265358979323846

#include <stb_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <math.h>
#include "camera.h"
#include "hsv2rgb.h"
#include "texture.h"
#include <vector>
#include "light.h"

float mixFactor = 0.0f;
float dt = 0.0f;
float lastFrame = 0.0f;

int viewportWidth = 1280;
int viewportHeight = 720;

float deg2rad(float deg) {
    return deg * (M_PI / 180.0f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    viewportWidth = width;
    viewportHeight = height;
    glViewport(0, 0, width, height);
}

struct vertex
{
    float x;
    float y;
    float z;

    float r;
    float g;
    float b;

    float u;
    float v;

    float nx;
    float ny;
    float nz;
};

/*
vertex vertices[] = {
    { -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },   // top-left (yellow)
    { -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // bottom-left (red)
    { 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },  // top-right (blue)
    { 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },  // bottom-right (green)
    //0.0f, 0.5f, 0.0f,
    
    //-0.35f, 0.1f, 0.0f,
    //-0.102, 0.22f, 0.0f,
    //-0.311, 0.35f, 0.0f,
    //0.07f, -0.265f, 0.0f,
    //0.223, -0.087f, 0.0f,
    //0.298, -0.272f, 0.0f
};
*/

vertex vertices[] = {
    { -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f },
    { 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f },
    { 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f },
    { 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f },
    { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f },

    { -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f },
    { 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f },
    { 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f },
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f },
    { -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f },
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f },

    { -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f },
    { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f },
    { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f },
    { -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f },
    { -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f },

    { 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f },
    { 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f },
    { 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f },
    { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f },
    { 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f },
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f },

    { -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },
    { 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
    { 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f },
    { 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f },
    { -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },

    { -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
    { 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
    { 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f }
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 1.0f, 0.0f, 1.0f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

unsigned int indices[] = {
    0, 1, 2,
    2, 3, 1
};

Camera camera;

float lastX = 1280 / 2;
float lastY = 720 / 2;

float sensitivity = 0.15f;

float moveSpeed = 15.0f;

float yaw = 0.0f;
float pitch = 0.0f;

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.handleMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        mixFactor = mixFactor + dt * 1.0f;
        
        if (mixFactor > 1.0f)
        {
            mixFactor = 1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        mixFactor = mixFactor - dt * 1.0f;

        if (mixFactor < 0.0f)
        {
            mixFactor = 0.0f;
        }
    }

    camera.handleKeyboard(window, dt);
}

std::vector<Light*> lights;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Learn OpenGL!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window :(" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, viewportWidth, viewportHeight);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Texture containerDiffuse("assets/container.png");
    Texture containerSpecular("assets/container_specular.png");

#pragma region Vertices
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)offsetof(vertex, x));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)offsetof(vertex, r));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)offsetof(vertex, u));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)offsetof(vertex, nx));
    glEnableVertexAttribArray(3);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#pragma endregion

    // Shader
    Shader litShader("./assets/shaders/lit.vert", "./assets/shaders/lit.frag");

    Shader unlitShader("./assets/shaders/unlit.vert", "./assets/shaders/unlit.frag");


    glm::vec3 lightColor = glm::vec3(0.3f);
    glm::vec3 diffuseColor = lightColor   * glm::vec3(1.0f); 
    glm::vec3 specularColor = lightColor   * glm::vec3(1.0f); 
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); 

    DirectionalLight* dirLight = new DirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), ambientColor, diffuseColor, specularColor);

    lightColor = hsv2rgb(180.0f * sin((float)glfwGetTime()) + 180.0f, 100.0f, 50.0f);

    diffuseColor = lightColor   * glm::vec3(1.5f); 
    specularColor = lightColor   * glm::vec3(1.5f); 
    ambientColor = diffuseColor * glm::vec3(0.2f); 

    PointLight* pointLight = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f), ambientColor, diffuseColor, specularColor);

    lightColor = hsv2rgb(180.0f * cos((float)glfwGetTime()) + 180.0f, 100.0f, 50.0f);

    diffuseColor = lightColor   * glm::vec3(1.5f); 
    specularColor = lightColor   * glm::vec3(1.5f); 
    ambientColor = diffuseColor * glm::vec3(0.2f); 

    PointLight* pointLight2 = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f), ambientColor, diffuseColor, specularColor);

    lightColor = glm::vec3(1.0f);

    diffuseColor = lightColor   * glm::vec3(1.0f); 
    specularColor = lightColor   * glm::vec3(1.0f); 
    ambientColor = diffuseColor * glm::vec3(0.2f); 
    
    SpotLight* spotLight = new SpotLight(camera.cameraPos, camera.front, ambientColor, diffuseColor, specularColor);

    lights.push_back(dirLight);
    lights.push_back(pointLight);
    lights.push_back(pointLight2);
    lights.push_back(spotLight);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentFrame = glfwGetTime();
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::string title_string = "Learn OpenGL! - FPS: ";
        title_string += std::to_string(1/dt);

        glfwSetWindowTitle(window, title_string.c_str());

        // wireframe mode
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        litShader.use();

        float time = glfwGetTime();
        float greenValue = (sin(time) / 2.0f) + 0.5f;
        //litShader.setFloat("green", greenValue);

        containerDiffuse.use(GL_TEXTURE0);
        containerSpecular.use(GL_TEXTURE1);

        pointLight->lightPos = glm::vec3(1.0 + cos((float)glfwGetTime()) * 1.5, 0.0, 1.0 + sin((float)glfwGetTime()) * 1.5);
        pointLight2->lightPos = glm::vec3(1.0 + cos((float)glfwGetTime()) * 1.5, 1.0 + sin((float)glfwGetTime()) * 1.5, -3.0);

        lightColor = hsv2rgb(180.0f * sin((float)glfwGetTime()) + 180.0f, 100.0f, 50.0f);

        diffuseColor = lightColor   * glm::vec3(1.0f); 
        specularColor = lightColor   * glm::vec3(1.0f); 
        ambientColor = diffuseColor * glm::vec3(0.2f); 

        pointLight->diffuse = diffuseColor;
        pointLight->specular = specularColor;
        pointLight->ambient = ambientColor;

        lightColor = hsv2rgb(180.0f * cos((float)glfwGetTime()) + 180.0f, 100.0f, 50.0f);

        diffuseColor = lightColor   * glm::vec3(1.0f); 
        specularColor = lightColor   * glm::vec3(1.0f); 
        ambientColor = diffuseColor * glm::vec3(0.2f); 

        pointLight2->diffuse = diffuseColor;
        pointLight2->specular = specularColor;
        pointLight2->ambient = ambientColor;

        spotLight->lightPos = camera.cameraPos;
        spotLight->lightDirection = camera.front;

        litShader.setInt("lightCounts", lights.size());
        for (size_t i = 0; i < lights.size(); i++)
        {
            Light* light = lights[i];

            std::string val = "lights[";
            val += std::to_string(i);
            val += "]";

            //std::cout << val << " - " << light->lightType << std::endl;
            
            litShader.setInt(val + ".lightType", light->lightType);
            litShader.setVec3(val + ".direction", light->lightDirection);
            litShader.setVec3(val + ".position", light->lightPos);

            litShader.setFloat(val + ".constant", light->constant);
            litShader.setFloat(val + ".linear", light->linear);
            litShader.setFloat(val + ".quadratic", light->quadratic);

            litShader.setFloat(val + ".cutoff", glm::cos(glm::radians(light->cutoffDegree)));
            litShader.setFloat(val + ".outerCutoff", glm::cos(glm::radians(light->outerCutoffDegree)));

            litShader.setVec3(val + ".ambient", light->ambient);
            litShader.setVec3(val + ".diffuse", light->diffuse);
            litShader.setVec3(val + ".specular", light->specular);
        }
        

        //litShader.setInt("ourTexture", 0);
        //litShader.setInt("ourTexture2", 1);

        //litShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        //litShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        litShader.setInt("material.diffuse", 0);
        litShader.setInt("material.specular", 1);
        litShader.setFloat("material.shininess", 32.0f);

        // glm::vec3 lightPos = glm::vec3(1.0 + cos((float)glfwGetTime()) * 1.5, 0.0, 1.0 + sin((float)glfwGetTime()) * 1.5);
        // //glm::vec3 lightColor = hsv2rgb(180.0f * sin((float)glfwGetTime()) + 180.0f, 100.0f, 50.0f);
        // glm::vec3 lightColor = glm::vec3(1.0f);

        // glm::vec3 diffuseColor = lightColor   * glm::vec3(1.0f); 
        // glm::vec3 specularColor = lightColor   * glm::vec3(1.0f); 
        // glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); 
        
        // litShader.setVec3("light.position", lightPos);
        // litShader.setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

        // litShader.setFloat("light.constant", 1.0f);
        // litShader.setFloat("light.linear", 0.09f);
        // litShader.setFloat("light.quadratic", 0.032f);
        
        // litShader.setVec3("light.ambient", ambientColor);
        // litShader.setVec3("light.diffuse", diffuseColor);
        // litShader.setVec3("light.specular", specularColor);

        // litShader.setInt("light.lightType", 2);
        // litShader.setVec3("light.position", camera.cameraPos);
        // litShader.setVec3("light.direction", camera.front);
        // litShader.setFloat("light.cutoff", glm::cos(glm::radians(15.5f)));
        // litShader.setFloat("light.outerCutoff", glm::cos(glm::radians(30.0f)));
        
        //std::cout << 360 * sin((float)glfwGetTime()) << std::endl;

        //std::cout << mixFactor << std::endl;
        //litShader.setFloat("mixFactor", mixFactor);
        //litShader.setVec4("lightColor", glm::vec4(lightColor.x, lightColor.y, lightColor.z, 1.0));
        //litShader.setVec3("lightPos", lightPos);
        litShader.setVec3("viewPos", camera.cameraPos);
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        //float radius = 10.0f;
        //cameraPos.x = sin(glfwGetTime()) * radius;
        //cameraPos.z = cos(glfwGetTime()) * radius;

        glm::mat4 view = camera.getViewMatrix();

        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(90.0f), (float)viewportWidth/(float)viewportHeight, 0.1f, 100.0f);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)));
        litShader.setMat4("view", view);
        litShader.setMat4("projection", projection);

        for (int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);

            if (i % 3 == 0) {
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            }

            litShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        for (int i = 0; i < lights.size(); i++)
        {
            Light* light = lights[i];

            if (light->lightType != LIGHT_POINT) continue;

            unlitShader.use();

            unlitShader.setMat4("view", view);
            unlitShader.setMat4("projection", projection);
            unlitShader.setVec4("ourColor", glm::vec4(light->diffuse.x, light->diffuse.y, light->diffuse.z, 1.0));

            glm::mat4 model(1.0f);
            model = glm::translate(model, light->lightPos);
            model = glm::scale(model, glm::vec3(0.15f));

            /*
            if (i % 3 == 0) {
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            */

            unlitShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        //glBindVertexArray(VAO);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}