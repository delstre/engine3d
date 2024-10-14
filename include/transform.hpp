#pragma once

#include <component.hpp>
#include <regclass.hpp>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

namespace Engine {
    class Transform : public Component {
        public:
            ~Transform() {};

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
        private:
            glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
            glm::vec3 angle = glm::vec3(0.0, 0.0, 0.0);
            glm::mat4 matrix = glm::mat4(1.0f);
    };

    REGISTER_TYPE(Transform, Component);
}


REFL_AUTO(
    type(Engine::Transform, bases<Engine::Component>), 
    func(GetPosition, property("Position")),
    func(GetAngle, property("Angle")),
    field(isEnabled)
);

REFL_AUTO(
    type(glm::vec3),
    field(x),
    field(y),
    field(z)
);

