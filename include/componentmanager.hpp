#pragma once

#include <unordered_map>
#include <functional>
#include <string>

namespace Engine {
    class Object;
    class IComponent;

    namespace ComponentManager {
        void RegisterComponent(const std::string& name, std::function<IComponent*()> constructor);
        void RegisterComponents();
        std::unordered_map<std::string, std::function<IComponent*()>> GetConstructors();
        IComponent* CreateComponent(const std::string& name);

        static std::unordered_map<std::string, std::function<IComponent*()>> constructors;
    };
}

#define REGISTER_COMPONENT(NAME) \
    NAME* NAME##_PTR = new NAME(); \
    Engine::ComponentManager::RegisterComponent(NAME##_PTR->GetTypeName(), []() { return new NAME(); }); \
    delete NAME##_PTR; \
