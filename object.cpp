#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <IL/il.h>

#include "object.hpp"
#include "modelinstance.hpp"

#include <iostream>

using namespace Renderer;

Object::Object() : Object(0, 0, 0) {}

Object::Object(int x, int y, int z) {
    SetPosition(x, y, z);
}

Object::Object(Model* model, int x, int y, int z) : Object(x, y, z) {
    SetModel(model);
}

Object::Object(ModelInstance* model, int x, int y, int z) : Object(x, y, z) {
    SetModelInstance(model);
}

void Object::SetModel(Model* model) {
    this->pModelInstance = nullptr;
    this->pModel = model;
}

Model* Object::GetModel() {
    return pModel;
}

void Object::SetModelInstance(ModelInstance* model) {
    this->pModel = nullptr;
    this->pModelInstance = model;
}

ModelInstance* Object::GetModelInstance() {
    return pModelInstance;
}

void Object::SetPosition(glm::vec3 position) {
    SetPosition(position.x, position.y, position.z);
}

void Object::SetPosition(int x, int y, int z) {
    position = glm::vec3(x, y, z);
    UpdatePosition();
}

glm::vec3& Object::GetPosition() {
    return position;
}

glm::mat4& Object::GetMatrix() {
    return matmodel;
}

void Object::SetColor(glm::vec3 color) {
    this->color = color;
}

glm::vec3& Object::GetColor() {
    return color;
}

void Object::SetTexture(uint texture) {
    this->texture = texture;
}

GLuint Object::GetTexture() {
    return texture;
}

void Object::SetTextureInstance(int id, uint texture) {
    if (pModelInstance != nullptr) {
        pModelInstance->GetTextures()[id] = texture;
        pModelInstance->UpdateTextures();
    }
}

GLuint Object::GetTextureInstance(int id) {
    if (pModelInstance != nullptr) {
        return pModelInstance->GetTextures()[id];
    }

    return 0;
}

void Object::UpdatePosition() {
    matmodel = glm::translate(glm::mat4(1.0f), position);
}

bool Object::ModelIsInstanced() {
    return pModelInstance != nullptr;
}

glm::vec3 Object::GetMinBounds() {
    if (pModel != nullptr) {
        return pModel->GetMinBounds() + position;
    }

    return position;
}

glm::vec3 Object::GetMaxBounds() {
    if (pModel != nullptr) {
        return pModel->GetMaxBounds() + position;
    }

    return position;
}

void Object::Render(const glm::mat4 mvp, const std::vector<GLuint>& textures) {
    if (IsActive == false)
        return;
 
    if (pModel != nullptr)
        pModel->Render(mvp, matmodel, texture);
    
    if (pModelInstance != nullptr)
        pModelInstance->Render(mvp, textures);
}

Renderer::ModelInstance* Renderer::TranslateModelsToInstance(std::vector<Renderer::Object*>& objects, int start, int end) {
    int size = objects.size();
    std::vector<glm::mat4> matrixes;
    std::vector<uint> textures;
    std::vector<glm::vec3> colors;

    for (int i = start; i < (end < size ? end : size); i++) {
        matrixes.push_back(objects[i]->GetMatrix());
        textures.push_back(objects[i]->GetTexture());
        colors.push_back(objects[i]->GetColor());
    }

    return new Renderer::ModelInstance(objects[start]->GetModel(), matrixes, textures, colors);
}
