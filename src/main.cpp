# define M_PI 3.14159265358979323846

#include "model.h"
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
    { 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f },
    { 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f },
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f },
    { -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f },
    { -0.5f, 0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f },

    { -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f },
    { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f },
    { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f },
    { -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f },
    { -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f },

    { 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f },
    { 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f },
    { 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f },
    { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f },
    { 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f },
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f },

    { -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },
    { 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
    { 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f },
    { 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f },
    { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
    { -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },

    { -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
    { 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
    { 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    { -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f }
};

Vertex planeVertices[] = {
    // Top-Left
    { glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0, 1.0) },
    
    // Top-Right
    { glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0, 1.0) },

    // Bottom-Left
    { glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0, 0.0) },
    
    // Bottom-Right
    { glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0, 0.0) }
};

unsigned int planeIndices[] = {
    1, 3, 2,
    2, 0, 1
};

Vertex screenQuadVertices[] = {
    // Top-Left
    { glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0, 1.0) },
    
    // Top-Right
    { glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0, 1.0) },

    // Bottom-Left
    { glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0, 0.0) },
    
    // Bottom-Right
    { glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0, 0.0) }
};

unsigned int screenQuadIndices[] = {
    1, 3, 2,
    2, 0, 1
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

    std::vector<Vertex> planeVerticesVec(std::begin(planeVertices), std::end(planeVertices));
    std::vector<unsigned int> planeIndicesVec(std::begin(planeIndices), std::end(planeIndices));

   Mesh planeMesh(planeVerticesVec, planeIndicesVec);

    std::vector<Vertex> screenQuadVerticesVec(std::begin(screenQuadVertices), std::end(screenQuadVertices));
    std::vector<unsigned int> screenQuadIndicesVec(std::begin(screenQuadIndices), std::end(screenQuadIndices));

   Mesh screenQuad(screenQuadVerticesVec, screenQuadIndicesVec);
   Model mdl("assets/intellij/intellij.obj");

    // Shader
    std::cout << "Lit Shader..." << std::endl;
    Shader litShader("./assets/shaders/lit.vert", "./assets/shaders/lit.frag");

    std::cout << "Grid Shader..." << std::endl;
    Shader gridShader("./assets/shaders/grid.vert", "./assets/shaders/grid.frag");

    std::cout << "Unlit Shader..." << std::endl;
    Shader unlitShader("./assets/shaders/unlit.vert", "./assets/shaders/unlit.frag");

    std::cout << "Screen Shader..." << std::endl;
    Shader screenShader("./assets/shaders/screen.vert", "./assets/shaders/screen.frag");

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

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportWidth, viewportHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentFrame = glfwGetTime();
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;


        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CW);


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::string title_string = "Learn OpenGL! - FPS: ";
        title_string += std::to_string(1/dt);

        glfwSetWindowTitle(window, title_string.c_str());

        // wireframe mode
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //litShader.use();

        float time = glfwGetTime();
        float greenValue = (sin(time) / 2.0f) + 0.5f;

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

        litShader.use();

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
        
        litShader.setInt("material.diffuse", 0);
        litShader.setInt("material.specular", 1);
        litShader.setFloat("material.shininess", 32.0f);

        litShader.setVec3("viewPos", camera.cameraPos);

        glm::mat4 view = camera.getViewMatrix();

        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(90.0f), (float)viewportWidth/(float)viewportHeight, 0.1f, 100.0f);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)));
        //litShader.setVec4("ourColor", glm::vec4(1.0)); 
        litShader.setMat4("view", view);
        litShader.setMat4("projection", projection);

        for (int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::scale(model, glm::vec3(1.0f));

            if (i % 3 == 0) {
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            }

            litShader.setMat4("model", model);
            mdl.draw(litShader);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        {
            gridShader.use();

            gridShader.setVec3("viewPos", camera.cameraPos);
            gridShader.setMat4("view", view);
            gridShader.setMat4("projection", projection);
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(camera.cameraPos.x, 0.0f, camera.cameraPos.z));
            model = glm::scale(model, glm::vec3(100.0f));
            gridShader.setMat4("model", model);
            planeMesh.draw(gridShader);
        }

        for (int i = 0; i < lights.size(); i++)
        {
            Light* light = lights[i];

            if (light->lightType != LIGHT_POINT) continue;

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

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

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // default fbo
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

        screenQuad.draw(screenShader);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);

    glfwTerminate();
    return 0;
}