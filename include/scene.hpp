#pragma once

#include "framebuffer.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "modelmanager.hpp"
#include "resourcemanager.hpp"
#include "controller.hpp"

namespace Engine {
    class Scene {
        public:
            void Init(GLFWwindow* pWindow);
            Renderer::Object* AddObject(std::string name="Cube", glm::vec3 position=glm::vec3(0, 0, 0));
            Renderer::Object* AddObject(Renderer::Object* obj);
            Renderer::Camera* AddCamera(glm::vec3 position); // maybe merge with object

            Renderer::Camera* GetActiveCamera();
            void DeleteObject(int id);

            void SetFrameSize(int width, int height);
            Renderer::FrameBuffer* GetFrameBuffer();

            std::vector<GLuint> GetTextures();

            std::vector<Renderer::Object*> GetObjects();

            void Render();

        private:
            Renderer::ModelManager* pModelManager = nullptr;
            Renderer::ResourceManager* pResourceManager = nullptr;
            Renderer::FrameBuffer* pFbo = nullptr;
            Renderer::Camera* pActiveCamera = nullptr;
            Engine::Controller* pController = nullptr; 
            GLFWwindow* pWindow = nullptr;
            
            std::vector<Renderer::Object*> objs;
            std::vector<Renderer::Object*> render;

            bool initialized = false;
    };
}
