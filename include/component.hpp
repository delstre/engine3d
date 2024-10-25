#pragma once

#include <icomponent.hpp>
#include <reflection.hpp>
#include <object.hpp>

#include <cassert>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

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

        private:
            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive& ar, const unsigned int version) {
                ar & isEnabled;
                ar & parent;
            }
    };
}

BOOST_CLASS_EXPORT_KEY(Engine::Component)
