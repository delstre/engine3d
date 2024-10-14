#pragma once

#include "regclass.hpp"
#include <icomponent.hpp>
#include <object.hpp>

#include <refl.hpp>

#include <map>

namespace Renderer {
    class Object;
}

namespace Engine {
    class Component : public IComponent {
        public:
            ~Component() {}
            void SetParent(Renderer::Object* parent);
            void UpdateInterface();

            void UpdateComponent() override;
            virtual void Start() override;
            virtual void End() override;

            virtual void Update();
            
            void SetEnable(bool enable);

            template<typename T>
            T* GetComponent();
            std::string GetTypeName() const;

            Renderer::Object* parent = nullptr;
            bool isEnabled = true;

    };
}

REFL_AUTO(
    type(Engine::Component),
    field(isEnabled)
);

REGISTER_BASE(Engine::Component);

