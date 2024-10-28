#pragma once

#include <component.hpp>
#include <modelmanager.hpp>
#include <mesh.hpp>

namespace Renderer {
    class ShaderProgram;
    class Mesh;
}

namespace Engine {
    class Model: public Engine::Component {
        public:
            Model();
            ~Model() {};

            void SetModel(Renderer::Mesh* mesh);
            void SetColor(glm::vec3 color);

            void Update();
            void InterfaceUpdate();
            void Start();
            void End();

            glm::vec3 GetPosition();
            glm::mat4 GetMatrix();
            glm::vec3 GetMins();
            glm::vec3 GetMaxs();

            DECLARE_CLASS_VARIABLE(Renderer::Mesh*, mesh, nullptr);

            DECLARE_CLASS_VARIABLES(
                REGISTER_CLASS_VARIABLE(Renderer::Mesh*, mesh);
            )
    };
}
