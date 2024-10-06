#include <model.hpp>
#include <modelinstance.hpp>

namespace Engine {
    class Component {
        public:
            virtual ~Component() {}
            virtual void Update() = 0;
    };

    class Transform : public Component {
        public:
            glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
            glm::vec3 angle = glm::vec3(0.0, 0.0, 0.0);

            void Update() override {
                //std::cout << "Transform::Update()" << std::endl;
            }
    };

    class ModelRender : public Component {
        public:
            Renderer::Model* model = nullptr;

            void Update() override {
                //model->Render(envy, matmodel, texture);
            }
    };

    class ModelInstancedRender : public Component {
        public:
            Renderer::ModelInstance* model = nullptr;

            void Update() override {
                //model->Render(envy, textures);
            }
    };

    class Physics : public Component {
        public:
            void Update() override {
                //std::cout << "Physics::Update()" << std::endl;
            }
    };

    class Controller : public Component {
        public:
            void Update() override {
                //std::cout << "Controller::Update()" << std::endl;
            }
    };
}
