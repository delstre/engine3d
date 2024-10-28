#pragma once

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

namespace Renderer {
    class FrameBuffer;
}

namespace Engine {
    class Object;
    class Camera;
    class WindowController;

    class Scene {
        public:
            void Init(GLFWwindow* pWindow);
            void SetPath(std::string path);
            void AddObject(Engine::Object* obj);
            void DeleteObject(Engine::Object* id);
            Engine::Camera* AddCamera(glm::vec3 position); // maybe merge with object

            void SetFrameSize(int width, int height);

            bool IsHitByRay(glm::vec3 origin, glm::vec3 direction, Object* obj);
            bool IsHitByRay(glm::vec3 origin, glm::vec3 direction, float& tNear, float& tFar);

            Engine::Camera* GetActiveCamera();
            Renderer::FrameBuffer* GetFrameBuffer();
            std::vector<GLuint> GetTextures();
            std::vector<Engine::Object*> GetObjects();
            GLFWwindow* GetWindow();

            float GetAspectRatio();

            void Render();
            int width;
            int height;

            std::vector<Engine::Object*> objs;

        private:
            std::string path;
            std::vector<std::string> comps;

            Renderer::FrameBuffer* pFbo = nullptr;
            Engine::Camera* pActiveCamera = nullptr;
            Engine::WindowController* pController = nullptr; 
            GLFWwindow* pWindow = nullptr;
            
            std::vector<Engine::Object*> render;

            bool initialized = false;
    };
}
