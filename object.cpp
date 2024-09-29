#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <IL/il.h>

#include "object.hpp"

using namespace Renderer;

Object::Object() : Object(0, 0, 0) {}
Object::Object(int x, int y, int z) : activeFaces(7) {
    SetPosition(x, y, z);
    UpdateActiveFaces();
}

Object::Object(Model* model, int x, int y, int z) : Object(x, y, z) {
    SetModel(model);
}

void Object::SetModel(Model* model) {
    this->pModel = model;
}

void Object::SetModelColor(float r, float g, float b) {
    pModel->UpdateColors(glm::vec3(r, g, b));
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

    pModel->Render(mvp, matmodel, texture);
}

void Object::AddActiveFaces(Face face, Object* object) {
    if (object == this || object == nullptr)
        return;

    if (activeFaces[face] != nullptr || object->activeFaces[face] != nullptr)
        return;

    activeFaces[face] = object;

    if (face == FRONT) {
        object->AddActiveFaces(BACK, this);
    } else if (face == BACK) {
        object->AddActiveFaces(FRONT, this);
    } else if (face == LEFT) {
        object->AddActiveFaces(RIGHT, this);
    } else if (face == RIGHT) {
        object->AddActiveFaces(LEFT, this);
    } else if (face == TOP) {
        object->AddActiveFaces(BOTTOM, this);
    } else if (face == BOTTOM) {
        object->AddActiveFaces(TOP, this);
    }

    UpdateActiveFaces();
    object->UpdateActiveFaces();
}

void Object::UpdateActiveFaces() {
    IsActive = false;

    // Проверяем каждую грань на активность
    for (int i = 0; i < 6; i++) {
        if (activeFaces[i] != nullptr) {
            IsActive = true;
            return; // Если хотя бы одна грань активна, выходим из функции
        }
    }
}
