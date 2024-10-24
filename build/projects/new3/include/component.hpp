#pragma once

#include <cassert>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include <icomponent.hpp>
#include <reflection.hpp>
#include <object.hpp>

namespace Renderer {
    class Object;
}

namespace Engine {
    class Component : public IComponent {
        public:
            ~Component() = default;
            Component() { Init(); }
            void SetParent(Renderer::Object* parent);
            Renderer::Object* GetParent() const;
            void UpdateInterface();

            void UpdateComponent() override;
            virtual void Start() override;
            virtual void End() override;

            virtual void Update();
            
            void SetEnable(bool enable);

            template<typename T>
            T* GetComponent();
            std::string GetTypeName() const;

            DECLARE_CLASS_VARIABLE(Renderer::Object*, parent, nullptr);
            DECLARE_CLASS_VARIABLE(bool, isEnabled, true);

            DECLARE_VARIABLES_VECTOR();

            DECLARE_CLASS_VARIABLES(
                REGISTER_CLASS_VARIABLE(Renderer::Object*, parent);
                REGISTER_CLASS_VARIABLE(bool, isEnabled);
            )

        private:
            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive& ar, const unsigned int version) {
                ar & isEnabled;
            }
    };
}
