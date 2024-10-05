#pragma once

#include "object.hpp"
#include "camera.hpp"

namespace Engine {
    class Scene {
        public:
            AddObject(Renderer::Object* obj, glm::vec3 position);
            AddCamera(Renderer::Camera* cam, glm::vec3 position);
            DeleteObject(int id);

        private:
            Renderer::ModelManager* mdlManager;
            Renderer::ResourceManager* resManager;

            std::vector<Renderer::Object*> objs;
            std::vector<Renderer::Object*> render;
    };
}
