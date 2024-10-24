#pragma once

#include "framebuffer.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "modelmanager.hpp"
#include "resourcemanager.hpp"
#include "controller.hpp"

#include <dlfcn.h>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

namespace Engine {
    class Scene {
        public:
            void Init(GLFWwindow* pWindow);
            void SetPath(std::string path);
            void AddObject(Renderer::Object* obj);
            Renderer::Camera* AddCamera(glm::vec3 position); // maybe merge with object

            Renderer::Camera* GetActiveCamera();
            void DeleteObject(int id);

            void SetFrameSize(int width, int height);
            Renderer::FrameBuffer* GetFrameBuffer();

            void SetComponentManager(Engine::ComponentManager* manager);

            std::vector<GLuint> GetTextures();

            std::vector<Renderer::Object*> GetObjects();

            Renderer::ModelManager* GetModelManager();
            Renderer::ResourceManager* GetResourceManager();

            void Render();

            Engine::ComponentManager* pComponentManager = nullptr;

        private:
            std::string path;
            std::vector<std::string> comps;

            Renderer::ModelManager* pModelManager = nullptr;
            Renderer::ResourceManager* pResourceManager = nullptr;
            Renderer::FrameBuffer* pFbo = nullptr;
            Renderer::Camera* pActiveCamera = nullptr;
            Engine::WindowController* pController = nullptr; 
            GLFWwindow* pWindow = nullptr;
            
            std::vector<Renderer::Object*> objs;
            std::vector<Renderer::Object*> render;

            bool initialized = false;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive& ar, const unsigned int version) {
                ar & objs;
            }
    };
}
