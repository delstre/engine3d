#include "camera.hpp"
#include "shaderprogram.hpp"

#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#include <GLFW/glfw3.h>

#include <iostream>

using namespace Renderer;

Camera::Camera(ShaderProgram* shaderProgram, glm::ivec2* winSize) {
    pShaderProgram = shaderProgram;
    pWinSize = winSize; 
    Render();
};

float Camera::SpectAxis() {
    float screen_width = pWinSize->x;
    float screen_height = pWinSize->y;

    float outputzoom = 1.0f;
    float aspectorigin = 16.0f / 9.0f;
    int aspectconstraint = 1;
    switch (aspectconstraint) {
        case 1:
            if ((screen_width / screen_height) < aspectorigin) {
                outputzoom *= ((screen_width / screen_height) / aspectorigin);
            }
            else {
                outputzoom *= ((float)aspectorigin / aspectorigin);
            }
            break;
        case 2:
            outputzoom *= ((screen_width / screen_height) / aspectorigin);
            break;
        default:
            outputzoom *= ((float)aspectorigin / aspectorigin);
    }

    return outputzoom;
}

float Camera::ReCalculateFOV() {
    return 2.0f * glm::atan(glm::tan(glm::radians(45.0f / 2.0f)) / SpectAxis());
}

glm::vec3 Camera::getCursor3DPos(double x, double y) {
    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);
    glm::vec4 rayEye = glm::inverse(projection) * rayClip; // Примените обратную проекцию
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // Установите Z=-1 и W=0
    glm::vec3 rayWorld = glm::vec3(glm::inverse(view) * rayEye); // Примените обратную матрицу вида
    glm::vec3 rayDirection = glm::normalize(rayWorld); // Нормализуйте направление луча

    return rayDirection;
} 

void Camera::Render() {
    projection = glm::perspective(ReCalculateFOV(), 1.0f * pWinSize->x / pWinSize->y, 0.1f, 200.0f);
    pShaderProgram->setMatrix4("projection", projection);

    view = glm::lookAt(position, position + front, up);
    pShaderProgram->setMatrix4("view", view);

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Обновляем векторы right и up
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Ограничиваем угол поворота камеры по вертикали
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        if (yaw > 360.0f)
            yaw = 0.0f;
        if (yaw < -360.0f)
            yaw = 0.0f;
    }
}

void Camera::ProcessKeyboardInput(int direction, float deltaTime) {
    float velocity = speed * deltaTime;
    if (direction == GLFW_KEY_W) // Вперёд
        position += front * velocity;
    if (direction == GLFW_KEY_S) // Назад
        position -= front * velocity;
    if (direction == GLFW_KEY_A) // Влево
        position -= right * velocity;
    if (direction == GLFW_KEY_D) // Вправо
        position += right * velocity;
}

void Camera::Control(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        ProcessKeyboardInput(GLFW_KEY_W, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ProcessKeyboardInput(GLFW_KEY_S, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ProcessKeyboardInput(GLFW_KEY_A, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ProcessKeyboardInput(GLFW_KEY_D, deltaTime);
}
