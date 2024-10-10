#pragma once

#include <imgui.h>
#include <object.hpp>

#include <functional>
#include <memory>

namespace Renderer {
    class Object;
}

namespace Engine {
    class Component {
        public:
            virtual ~Component() {}
            virtual void Update() = 0;
            virtual void InterfaceUpdate() = 0;

            void SetParent(Renderer::Object* parent);

            template <typename T> T* GetComponent();
            std::string GetTypeName();

        protected:
            Renderer::Object* parent = nullptr;
    };

    class ComponentManager {
        public:
            using ComponentConstructor = std::function<Component*(Renderer::Object*)>;

            void RegisterComponent(const std::string& name, ComponentConstructor constructor);
            void RegisterComponents();
            std::unordered_map<std::string, ComponentConstructor> GetComponents();
            Component* CreateComponent(Renderer::Object* obj, const std::string& name);

        private:
            std::unordered_map<std::string, ComponentConstructor> constructors;
    };
}
