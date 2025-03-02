#ifndef CAMERA_H
#define CAMERA_H

#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
public:
    glm::vec3 cameraPos;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 front;
    glm::vec3 worldUp;

    float pitch;
    float yaw;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float movementSpeed = SPEED,
            float sensitivty = SENSITIVITY, float zoom = ZOOM) {
        
                this->cameraPos = pos;
                this->worldUp = up;
                this->yaw = yaw;
                this->pitch = pitch;
                this->movementSpeed = movementSpeed;
                this->mouseSensitivity = sensitivty;
                this->zoom = zoom;

                updateCameraVectors();
    }

    void handleMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
        {
            pitch = 89.0f;
        }

        if (pitch < -89.0f)
        {
            pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void handleKeyboard(GLFWwindow* window, float dt)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos = cameraPos + front * movementSpeed * dt;
        }
        
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos = cameraPos + front * -movementSpeed * dt;
        }
    
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPos = cameraPos + right * -movementSpeed * dt;
        }
    
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos = cameraPos + right * movementSpeed * dt;
        }

        updateCameraVectors();
    }

    glm::mat4 getViewMatrix()
    {
        glm::mat4 view(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + front, up);

        return view;
    }

private:
    void updateCameraVectors()
    {
        glm::vec3 front(0.0f);
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        this->front = glm::normalize(front);

        this->right = glm::normalize(glm::cross(front, worldUp));
        this->up = glm::cross(right, front);
    }
};

#endif