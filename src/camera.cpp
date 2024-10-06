#include "camera.hpp"

#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include <cfloat>

using namespace Renderer;

Camera::Camera(GLFWwindow* window) {
    pWindow = window;
    
    UpdateWindowSize();
    UpdateMVP();
};

void Camera::UpdateWindowSize() {
    glfwGetWindowSize(pWindow, &wid, &hei);
}

void Camera::UpdateMVP() {
    projection = glm::perspective(ReCalculateFOV(), 1.0f * wid / hei, 0.1f, 100.0f);
    view = glm::lookAt(position, position + front, up);
    mvp = projection * view;
}

void Camera::UpdateFrustum() {
    float* m = glm::value_ptr(mvp);

    // Плоскости
    // Near
    planes[0] = glm::vec4(m[3] + m[2], m[7] + m[6], m[11] + m[10], m[15] + m[14]);
    // Far
    planes[1] = glm::vec4(m[3] - m[2], m[7] - m[6], m[11] - m[10], m[15] - m[14]);
    // Left
    planes[2] = glm::vec4(m[3] + m[0], m[7] + m[4], m[11] + m[8], m[15] + m[12]);
    // Right
    planes[3] = glm::vec4(m[3] - m[0], m[7] - m[4], m[11] - m[8], m[15] - m[12]);
    // Top
    planes[4] = glm::vec4(m[3] - m[1], m[7] - m[5], m[11] - m[9], m[15] - m[13]);
    // Bottom
    planes[5] = glm::vec4(m[3] + m[1], m[7] + m[5], m[11] + m[9], m[15] + m[13]);

    // Извлечение плоскостей
    //planes[0] = glm::vec4(clip[0][3] + clip[0][0], clip[1][3] + clip[1][0], clip[2][3] + clip[2][0], clip[3][3] + clip[3][0]); // Левая плоскость
    //planes[1] = glm::vec4(clip[0][3] - clip[0][0], clip[1][3] - clip[1][0], clip[2][3] - clip[2][0], clip[3][3] - clip[3][0]); // Правая плоскость
    //planes[2] = glm::vec4(clip[0][3] + clip[0][1], clip[1][3] + clip[1][1], clip[2][3] + clip[2][1], clip[3][3] + clip[3][1]); // Нижняя плоскость
    //planes[3] = glm::vec4(clip[0][3] - clip[0][1], clip[1][3] - clip[1][1], clip[2][3] - clip[2][1], clip[3][3] - clip[3][1]); // Верхняя плоскость
    //planes[4] = glm::vec4(clip[0][3] + clip[0][2], clip[1][3] + clip[1][2], clip[2][3] + clip[2][2], clip[3][3] + clip[3][2]); // Ближняя плоскость
    //planes[5] = glm::vec4(clip[0][3] - clip[0][2], clip[1][3] - clip[1][2], clip[2][3] - clip[2][2], clip[3][3] - clip[3][2]); // Дальняя плоскость

    // Нормализуем плоскости для упрощения дальнейших расчетов
    //for (int i = 0; i < 6; i++) {
        //float length = glm::length(glm::vec3(planes[i]));
        //planes[i] /= length;
    //}
}

bool Camera::IsBoxInFrustum(const glm::vec3& min, const glm::vec3& max) {
    //for (int i = 0; i < 6; i++) {
        //glm::vec3 positiveVertex = min;

        //if (planes[i].x >= 0) positiveVertex.x = max.x;
        //if (planes[i].y >= 0) positiveVertex.y = max.y;
        //if (planes[i].z >= 0) positiveVertex.z = max.z;

        //if (glm::dot(glm::vec3(planes[i]), positiveVertex) + planes[i].w < 0)
            //return false;
    //}
    //return true;
}

bool Camera::IsObjectInFrustum(Object* object) {
    //for (int i = 0; i < 6; i++) {
        //float distance = glm::dot(glm::vec3(planes[i]), object->position) + planes[i].w;
        //if (distance < -2) {
            //return false; // Объект вне фрустра
        //}
    //}
    return true; // Объект внутри фрустра
}

float Camera::SpectAxis() {
    float screen_width = wid;
    float screen_height = hei; 

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

bool Camera::RayIntersectsBox(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& minBounds, const glm::vec3& maxBounds, float& tNear, float& tFar) {
    //glm::vec3 invDir = 1.0f / direction;
    //glm::vec3 t0 = (boxMin - origin) * invDir;
    //glm::vec3 t1 = (boxMax - origin) * invDir;

    //glm::vec3 tMin = glm::min(t0, t1);
    //glm::vec3 tMax = glm::max(t0, t1);

    //float tEntry = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
    //float tExit = glm::min(glm::min(tMax.x, tMax.y), tMax.z);
    
    //return tEntry <= tExit && tExit >= 0;
    tNear = -FLT_MAX;
    tFar = FLT_MAX;

    // Проверка по каждой оси (X, Y, Z)
    for (int i = 0; i < 3; i++) {
        if (rayDir[i] != 0.0f) {
            // Найдем расстояния до границ по этой оси
            float t1 = (minBounds[i] - rayOrigin[i]) / rayDir[i];
            float t2 = (maxBounds[i] - rayOrigin[i]) / rayDir[i];

            // Убедимся, что t1 — минимальное, а t2 — максимальное
            if (t1 > t2) std::swap(t1, t2);

            // tNear — максимальное из всех t1, а tFar — минимальное из всех t2
            tNear = std::max(tNear, t1);
            tFar = std::min(tFar, t2);

            // Если tNear больше tFar, то пересечения нет
            if (tNear > tFar) return false;
        } else {
            // Если луч параллелен оси, то луч должен быть внутри границ по этой оси
            if (rayOrigin[i] < minBounds[i] || rayOrigin[i] > maxBounds[i]) return false;
        }
    }

    // Если мы дошли до этого момента, значит луч пересекает куб
    return true;
}

bool Camera::IsHitByRay(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& position) {
    // Простейшая проверка: пересечение луча с шаром (можно улучшить)
    glm::vec3 oc = rayOrigin - position;
    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot(oc, rayDirection);
    float c = glm::dot(oc, oc) - 1 * 1;
    float discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}


//Face Camera::GetIntersectedFace(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& boxMin, const glm::vec3& boxMax) {
    //std::vector<Face> faces = {FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM};
    //std::vector<glm::vec3> faceNormals = {
        //glm::vec3(0, 0, 1),   // FRONT
        //glm::vec3(0, 0, -1),  // BACK
        //glm::vec3(-1, 0, 0),  // LEFT
        //glm::vec3(1, 0, 0),   // RIGHT
        //glm::vec3(0, 1, 0),   // TOP
        //glm::vec3(0, -1, 0)   // BOTTOM
    //};

    //for (size_t i = 0; i < faces.size(); ++i) {
        //glm::vec3 normal = faceNormals[i];
        //glm::vec3 facePoint;

        //if (normal.z == 1) {
            //facePoint = glm::vec3(boxMin.x, boxMin.y, boxMax.z); // FRONT
        //} else if (normal.z == -1) {
            //facePoint = glm::vec3(boxMin.x, boxMin.y, boxMin.z); // BACK
        //} else if (normal.y == -1) {
            //facePoint = glm::vec3(boxMin.x, boxMax.y, boxMin.z); // TOP
        //} else if (normal.y == 1) {
            //facePoint = glm::vec3(boxMin.x, boxMin.y, boxMin.z); // BOTTOM
        //} else if (normal.x == -1) {
            //facePoint = glm::vec3(boxMin.x, boxMin.y, boxMin.z); // LEFT
        //} else if (normal.x == 1) {
            //facePoint = glm::vec3(boxMin.x, boxMin.y, boxMax.z); // RIGHT
        //}

        ////std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
        ////std::cout << rayDirection.x << " " << rayDirection.y << " " << rayDirection.z << std::endl;
        ////std::cout << boxMin.x << " " << boxMin.y << " " << boxMin.z << std::endl;
            
        //int dir = 0;
        //glm::vec3 dirv = abs(facePoint - rayOrigin);

        //if (dirv.y > dirv.x + dirv.z) {
            //if (i < 4) continue;
            //dir = 3;
        //} else if (dirv.x > dirv.z) {
            //dir = 1;
        //} else if (dirv.z > dirv.x) {
            //if (i < 2) continue;
            //dir = 2;
        //}

        ////std::cout << dir << " " << abs(dirv.x) << " " << dirv.y << " " << dirv.z << std::endl;

        //// Рассчитайте параметр t для пересечения
        //float t;
        //if (dir == 1) {
            //t = (facePoint.x - rayOrigin.x) / rayDirection.x;
        //} else if (dir == 2) {
            //t = (facePoint.z - rayOrigin.z) / rayDirection.z;
        //} else if (dir == 3) {
            //t = (facePoint.y - rayOrigin.y) / rayDirection.y;
        //} else {
            //continue;
        //}

        //// Проверка, попадает ли пересечение в границы куба
        //glm::vec3 intersection = rayOrigin + t * rayDirection;
        //if (intersection.x >= boxMin.x && intersection.x <= boxMax.x && 
            //intersection.y >= boxMin.y && intersection.y <= boxMax.y && 
            //intersection.z >= boxMin.z && intersection.z <= boxMax.z) {
        //std::cout << t << " " << intersection.x << " " << intersection.y << " " << intersection.z << std::endl;
            //std::cout << faces[i] << std::endl;
            //return faces[i];
        //} else {
            //continue;
        //}
    //}

    //return NONE;
//}

// Функция, получающая направление луча из координат мыши
glm::vec3 Camera::GetMouseRay(int mouseX, int mouseY, int windowWidth, int windowHeight, const glm::mat4& projection, const glm::mat4& view) {
    float x = (2.0f * mouseX) / windowWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / windowHeight;

    glm::vec4 rayClip = glm::vec4(x, y, -1.0, 1.0);
    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0); // z = -1 для направления
    glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));

    return rayWorld;
}

// Проверка пересечения
void Camera::CheckRayIntersection(GLFWwindow* window) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glm::vec3 origin = position; // Позиция камеры
    glm::vec3 direction = GetMouseRay(mouseX, mouseY, width, height, projection, view);
    
    // code for objects
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
