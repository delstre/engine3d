#include <transform.hpp>
#include <model.hpp>

#include "componentmanager.hpp"

using namespace Engine;

void ComponentManager::RegisterComponent(const std::string& name, std::function<Component*(Renderer::Object*)> constructor) {
    constructors[name] = constructor;
}

void ComponentManager::RegisterComponents() {
    ComponentManager::RegisterComponent("Transform", [](Renderer::Object* obj) {
        auto transform = new Transform;
        transform->SetParent(obj);
        return transform; 
    });

    ComponentManager::RegisterComponent("ModelRender", [](Renderer::Object* obj) { 
        auto model = new Renderer::ModelRender;
        model->SetParent(obj);
        return model;
    });
}

std::unordered_map<std::string, std::function<Component*(Renderer::Object*)>> ComponentManager::GetComponents() {
    return constructors;
}

Component* ComponentManager::CreateComponent(Renderer::Object* obj, const std::string& name) {
    if (constructors.find(name) != constructors.end()) {
        return constructors[name](obj);
    }
    return nullptr;
}
