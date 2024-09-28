#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <IL/il.h>

#include "object.hpp"

using namespace Renderer;

Object::Object() : Object(0, 0, 0) {}
Object::Object(int x, int y, int z) {
    SetPosition(x, y, z);
}

Object::Object(Model* model, int x, int y, int z) {
    SetModel(model);
    SetPosition(x, y, z);
}

void Object::SetModel(Model* model) {
    this->pModel = model;
}

void Object::SetPosition(glm::vec3 position) {
    SetPosition(position.x, position.y, position.z);
}

void Object::SetPosition(int x, int y, int z) {
    position = glm::vec3(x, y, z);
    UpdatePosition();
}

void Object::UpdatePosition() {
    matmodel = glm::translate(glm::mat4(1.0f), position);
}

void Object::SetTexture(const GLuint& texture) {
    this->texture = texture;
}

void Object::Render(const glm::mat4 mvp) {
    if (IsActive == false || pModel == nullptr)
        return;

    pModel->Render(mvp, matmodel, texture, false);
}
