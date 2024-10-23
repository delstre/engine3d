#include <transform.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

using namespace Engine;

Transform::Transform() : Component() { Init(); }
    
glm::vec3& Transform::GetPosition() {
    return position;
}

glm::vec3* Transform::GetPositionPtr() {
    return &position;
}

glm::vec3& Transform::GetAngle() {
    return angle;
}

glm::mat4& Transform::GetMatrix() { 
    return matrix; 
}

void Transform::SetPosition(glm::vec3 position) {
    this->position = position;
}

void Transform::SetAngle(glm::vec3 angle) {
    this->angle = angle;
}

void Transform::Start() {}
void Transform::End() {}

void Transform::Update() {
    matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(angle.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(angle.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(angle.z), glm::vec3(0.0f, 0.0f, 1.0f));
} 
