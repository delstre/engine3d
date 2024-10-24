#include <transform.hpp>
#include <model.hpp>

#include "componentmanager.hpp"

#include <iostream>

using namespace Engine;
using namespace Renderer;

void ComponentManager::RegisterComponent(const std::string& name, std::function<IComponent*()> constructor) {
    std::cout << "Registering component: " << name << std::endl;
    constructors[name] = constructor;
}

void ComponentManager::RegisterComponents() {
    REGISTER_COMPONENT(Transform);
    REGISTER_COMPONENT(Model);
}

std::unordered_map<std::string, std::function<IComponent*()>> ComponentManager::GetConstructors() {
    return constructors;
}

IComponent* ComponentManager::CreateComponent(const std::string& name) {
    if (constructors.find(name) != constructors.end()) {
        return constructors[name]();
    }
    return nullptr;
}


