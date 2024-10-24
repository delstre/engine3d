#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include <object.hpp>
#include <component.hpp>

namespace Renderer {
    class Object;
}

namespace Engine {
    namespace ComponentManager {
        void RegisterComponent(const std::string& name, std::function<IComponent*()> constructor);
        void RegisterComponents();
        std::unordered_map<std::string, std::function<IComponent*()>> GetConstructors();
        IComponent* CreateComponent(const std::string& name);

        static std::unordered_map<std::string, std::function<IComponent*()>> constructors;
    };
}

#define REGISTER_COMPONENT(NAME) \
    Engine::ComponentManager::RegisterComponent(#NAME, []() { return new NAME(); }); \
