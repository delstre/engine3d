#include <transform.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace Engine;
    
glm::vec3& Transform::GetPosition() {
    return position;
}

glm::mat4& Transform::GetMatrix() { 
    return matrix; 
}

void Transform::Update() {
    matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(angle.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(angle.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(angle.z), glm::vec3(0.0f, 0.0f, 1.0f));
} 

void Transform::InterfaceUpdate() {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Transform")) {
        ImGui::SliderFloat3("Position", glm::value_ptr(position), -10.0f, 10.0f);
        ImGui::SliderFloat3("Angle", glm::value_ptr(angle), -180.0f, 180.0f);

        ImGui::TreePop();
    }
}
