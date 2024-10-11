#pragma once

#include <component.hpp>

#include <glm/glm.hpp>

namespace Engine {
    class Transform : public Component {
        public:
            glm::mat4& GetMatrix();
            glm::vec3& GetPosition();

            void Update() override;
            void InterfaceUpdate() override;
        private:
            glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
            glm::vec3 angle = glm::vec3(0.0, 0.0, 0.0);
            glm::mat4 matrix = glm::mat4(1.0f);
    };
}
