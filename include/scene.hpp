#pragma once

#include <object.hpp>

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

            Engine::Camera* GetActiveCamera();

            void SetFrameSize(int width, int height);
            Renderer::FrameBuffer* GetFrameBuffer();

            std::vector<GLuint> GetTextures();

            std::vector<Engine::Object*> GetObjects();

            void Render();
        private:
            std::string path;
            std::vector<std::string> comps;

            Renderer::FrameBuffer* pFbo = nullptr;
            Engine::Camera* pActiveCamera = nullptr;
            Engine::WindowController* pController = nullptr; 
            GLFWwindow* pWindow = nullptr;
            
            std::vector<Engine::Object*> objs;
            std::vector<Engine::Object*> render;

            bool initialized = false;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive& ar, const unsigned int version) {
                ar & objs;
            }
    };
}
