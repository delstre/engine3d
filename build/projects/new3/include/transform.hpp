#pragma once

#include <component.hpp>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

namespace Engine {
    class Transform : public Component {
        public:
            ~Transform() {};
            Transform();

            glm::mat4& GetMatrix();
            glm::vec3& GetPosition();
            glm::vec3* GetPositionPtr();
            glm::vec3& GetAngle();

            void SetPosition(glm::vec3 position);
            void SetAngle(glm::vec3 angle);

            void Update();
            void InterfaceUpdate();
            void Start();
            void End();

            DECLARE_CLASS_VARIABLE(glm::vec3, position, glm::vec3(0.0, 0.0, 0.0));
            DECLARE_CLASS_VARIABLE(glm::vec3, angle, glm::vec3(0.0, 0.0, 0.0));
            glm::mat4 matrix = glm::mat4(1.0f);

            DECLARE_CLASS_VARIABLES(
                REGISTER_CLASS_VARIABLE(glm::vec3, position);
                REGISTER_CLASS_VARIABLE(glm::vec3, angle);
            )

        private:
            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive& ar, const unsigned int version) {
                ar & boost::serialization::base_object<Component>(*this);
            }
    };
}
