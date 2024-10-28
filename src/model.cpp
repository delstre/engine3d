#include <model.hpp>
#include <transform.hpp>
#include <mesh.hpp>

#include <fstream>
#include <modelmanager.hpp>
#include <iostream>

using namespace Engine;

Model::Model() : Component() {
    Init();
}

void Model::SetModel(Renderer::Mesh* model) {
    mesh = model;
}

void Model::Update() {
    if (mesh == nullptr) {
        return;
    }

    mesh->Render(GetParent()->env, 0, GetMatrix());
}

void Model::Start() {}
void Model::End() {}

void Model::SetColor(glm::vec3 color) {
    if (mesh) {
        mesh->SetColor(color);
    }
}

glm::vec3 Model::GetPosition() {
    auto transform = GetComponent<Engine::Transform>();
    if (transform) {
        return transform->GetPosition();
    }

    return glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::mat4 Model::GetMatrix() {
    auto transform = GetComponent<Engine::Transform>();
    if (transform) {
        return transform->GetMatrix();
    }

    return glm::mat4(1.0f);
}

glm::vec3 Model::GetMins() {
    if (mesh) {
        return GetPosition() + mesh->GetMins();
    }

    return glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 Model::GetMaxs() {
    if (mesh) {
        return GetPosition() + mesh->GetMaxs();
    }

    return glm::vec3(0.0f, 0.0f, 0.0f);
}
