#pragma once

#include <component.hpp>
#include <modelmanager.hpp>
#include <mesh.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/split_member.hpp>

namespace Renderer {
    class ShaderProgram;
    class Mesh;
}

namespace Engine {
    class Model: public Engine::Component {
        public:
            Model() { Init(); };
            ~Model() = default;

            void SetModel(Renderer::Mesh* mesh);

            void Update();
            void InterfaceUpdate();
            void Start();
            void End();

            DECLARE_CLASS_VARIABLE(Renderer::Mesh*, mesh, nullptr);

            DECLARE_CLASS_VARIABLES(
                REGISTER_CLASS_VARIABLE(Renderer::Mesh*, mesh);
            )
        private:
            friend class boost::serialization::access;

            template<class Archive>
            void save(Archive& ar, const unsigned int version) const {
                ar & boost::serialization::base_object<Component>(*this);
                ar & mesh; // only obj
            }

            template<class Archive>
            void load(Archive& ar, const unsigned int version) {
                ar & boost::serialization::base_object<Component>(*this);
                ar & mesh;
                this->mesh = ModelManager::GetModel(mesh->name);
            }

            BOOST_SERIALIZATION_SPLIT_MEMBER()
    };
}
