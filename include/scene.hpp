#pragma once

#include "framebuffer.hpp"
#include "object.hpp"
#include "camera.hpp"
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
            void DeleteObject(Renderer::Object* id);
            Renderer::Camera* AddCamera(glm::vec3 position); // maybe merge with object

            Renderer::Camera* GetActiveCamera();

            void SetFrameSize(int width, int height);
            Renderer::FrameBuffer* GetFrameBuffer();

            std::vector<GLuint> GetTextures();

            std::vector<Renderer::Object*> GetObjects();

            Renderer::ResourceManager* GetResourceManager();

            void Render();
        private:
            std::string path;
            std::vector<std::string> comps;

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

            //template<class Archive>
            //void load(Archive& ar, const unsigned int version) {
                //std::cout << "Loading Scene" << std::endl;
                //for (Renderer::Object* obj: objs) {
                    //AddObject(obj);
                //}
            //}
    };
}
