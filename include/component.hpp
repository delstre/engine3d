#pragma once

#include <icomponent.hpp>
#include <object.hpp>

#include <cassert>

namespace Engine {
    class Object;

    class Component : public IComponent {
        public:
            virtual ~Component() = default;
            Component() { Init(); }
            void SetParent(Engine::Object* parent);
            Engine::Object* GetParent() const;
            void UpdateInterface();

            void UpdateComponent() override;
            virtual void Start() override;
            virtual void End() override;

            virtual void Update();
            
            void SetEnable(bool enable);

            template<typename T>
            T* GetComponent();
            std::string GetTypeName() const;

            DECLARE_CLASS_VARIABLE(Engine::Object*, parent, nullptr);
            DECLARE_CLASS_VARIABLE(bool, isEnabled, true);

            DECLARE_VARIABLES_VECTOR();

            DECLARE_CLASS_VARIABLES(
                REGISTER_CLASS_VARIABLE(Engine::Object*, parent);
                REGISTER_CLASS_VARIABLE(bool, isEnabled);
            )
    };
}
