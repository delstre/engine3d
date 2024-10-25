#include <model.hpp>
#include <transform.hpp>
#include <mesh.hpp>

using namespace Engine;

void Model::SetModel(Renderer::Mesh* model) {
    mesh = model;
}

void Model::Update() {
    if (mesh == nullptr) {
        return;
    }

    auto transform = GetComponent<Engine::Transform>();
    auto mat = transform == nullptr ? glm::mat4(1.0f) : transform->GetMatrix();
    mesh->Render(GetParent()->env, 0, mat);
}

void Model::Start() {}
void Model::End() {}
