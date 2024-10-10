#include <component.hpp>
#include <transform.hpp>
#include <model.hpp>
#include <object.hpp>

#include <iostream>

using namespace Engine;

void Component::SetParent(Renderer::Object* parent) { 
    std::cout << "SetParent " << this << " " << parent << std::endl;
    if (this->parent == nullptr)
        this->parent = parent; 
}

template <typename T>
T* Component::GetComponent() { 
    return parent->template GetComponent<T>(); 
}

std::string Component::GetTypeName() {
    return typeid(*this).name();
}

void ComponentManager::RegisterComponent(const std::string& name, ComponentManager::ComponentConstructor constructor) {
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

std::unordered_map<std::string, ComponentManager::ComponentConstructor> ComponentManager::GetComponents() {
    return constructors;
}

Component* ComponentManager::CreateComponent(Renderer::Object* obj, const std::string& name) {
    if (constructors.find(name) != constructors.end()) {
        return constructors[name](obj);
    }
    return nullptr;
}


template Transform* Component::GetComponent<Transform>();
