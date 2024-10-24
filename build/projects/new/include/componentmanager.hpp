#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include <object.hpp>

namespace Renderer {
    class Object;
}

namespace Engine {
    class Component;

    class ComponentManager {
        public:
            ComponentManager() {};
            ~ComponentManager() = default; 

            void RegisterComponent(const std::string& name, std::function<IComponent*()> constructor);
            void RegisterComponents();
            std::unordered_map<std::string, std::function<IComponent*()>> GetConstructors();
            IComponent* CreateComponent(const std::string& name);

        private:
            std::unordered_map<std::string, std::function<IComponent*()>> constructors;
    };
}

#define REGISTER_COMPONENT(NAME) \
    ComponentManager::RegisterComponent(#NAME, []() { return new NAME(); }); \
