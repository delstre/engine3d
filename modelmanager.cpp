#include "modelmanager.hpp"

using namespace Renderer;

void ModelManager::AddModel(std::string name, Model* model) {
    models[name] = model;
}

Model* ModelManager::GetModel(std::string name) const {
    return models.at(name);
}
