#include <camera.hpp>
#include <object.hpp>
#include <scene.hpp>

using namespace Engine;

Camera::Camera(Scene* scene) {
    this->scene = scene;
    UpdateMVP();
};

void Camera::UpdateMVP() {
    projection = glm::perspective(glm::radians(zoom), scene->GetAspectRatio(), 0.1f, 100.0f);
    view = glm::lookAt(position, position + front, up);
    mvp = projection * view;
}

glm::vec3 Camera::GetCursorDirection(double mouseX, double mouseY, double width, double height) {
    float x = (2.0f * mouseX) / width - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / height;

    glm::vec4 rayClip(x, y, -1.0f, 1.0f); // Z = -1 на ближней плоскости (viewing plane)

    // Шаг 3: Преобразуем в пространство камеры (view space) с помощью обратной проекции
    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // В пространстве камеры луч идет вдоль -Z

    // Шаг 4: Преобразуем луч в мировое пространство (world space)
    glm::vec3 rayWorld = glm::vec3(glm::inverse(view) * rayEye);
    rayWorld = glm::normalize(rayWorld); // Нормализуем вектор направления

    return rayWorld; // Это направление луча от камеры в мировом пространстве
} 

void Camera::Think() {
    UpdateMVP();
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
