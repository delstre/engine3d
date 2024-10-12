#include <transform.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace Engine;
    
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

void Transform::Update() {
    matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(angle.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(angle.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(angle.z), glm::vec3(0.0f, 0.0f, 1.0f));
} 

void Transform::InterfaceUpdate() {
    ImGui::SliderFloat3("Position", glm::value_ptr(position), -10.0f, 10.0f);
    ImGui::SliderFloat3("Angle", glm::value_ptr(angle), -180.0f, 180.0f);
}

std::string GetTypeName() {
    return "Transform";
}

extern "C" {
    struct Positions {
        double x;
        int y;
        int z;
    };

    void* GetX(void* obj) {
        Transform* tr = static_cast<Renderer::Object*>(obj)->GetComponent<Transform>();

        if (tr) {
            Positions* p = new Positions;
            p->x = tr->GetPosition().x;
            p->y = tr->GetPosition().y;
            p->z = tr->GetPosition().z;
            return p;
        } else {
            return 0;
        }
    }
}
