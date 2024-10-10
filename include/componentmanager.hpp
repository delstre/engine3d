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
    class Component;

    class ComponentManager {
        public:
            ComponentManager() {};
            ~ComponentManager() = default; 

            void RegisterComponent(const std::string& name, std::function<Component*(Renderer::Object*)> constructor);
            void RegisterComponents();
            std::unordered_map<std::string, std::function<Component*(Renderer::Object*)>> GetComponents();
            Component* CreateComponent(Renderer::Object* obj, const std::string& name);

        private:
            std::unordered_map<std::string, std::function<Component*(Renderer::Object*)>> constructors;
    };
}
