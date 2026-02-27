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
    glm::vec4 ray_nds(x, y, -1.0f, 1.0f);
    glm::vec4 ray_eye = glm::inverse(projection) * ray_nds;
    ray_eye.z = -1.0f; ray_eye.w = 0.0f;
    glm::mat4 inv_view = glm::inverse(view);
    glm::vec3 dir_world = glm::normalize(glm::vec3(inv_view * ray_eye));
    return dir_world;
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

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

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
    if (direction == GLFW_KEY_W)
        position += front * velocity;
    if (direction == GLFW_KEY_S)
        position -= front * velocity;
    if (direction == GLFW_KEY_A)
        position -= right * velocity;
    if (direction == GLFW_KEY_D)
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
